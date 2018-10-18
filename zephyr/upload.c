/**
* @brief 
* @file upload.c
* @author J.H. 
* @date 2018-10-17
*/

/* system includes */
#include <zephyr.h>
#include <shell/shell.h>
#include <json.h>
#include <string.h>
#include <net/http.h>
#include <net/net_core.h>
#include <stdlib.h>

/* local includes */
#include "upload.h"
#include "config.h"
#include "http_utils.h"
#include "eth/sign.h"
#include "wallet.h"
#include "helpers/hextobin.h"
/*
 * Note that the http_ctx is quite large so be careful if that is
 * allocated from stack.
 */
static struct http_ctx http_ctx;

// header that contains the data signature
#define X_ANYLEDGER_SIG "X-Anyledger-Sig"
typedef struct {
    uint8_t field_name[sizeof(X_ANYLEDGER_SIG)-1];
    uint8_t separator[sizeof(": ")-1];
    uint8_t signature[130];
    uint8_t terminator[sizeof("\n")];
} anyledger_signature_hdr_t;

static anyledger_signature_hdr_t _sig_hdr = {
    .field_name = X_ANYLEDGER_SIG,
    .separator = {':', ' '},
    .signature = {0},
    .terminator ="\n"
};

#if defined(CONFIG_NET_CONTEXT_NET_PKT_POOL)
NET_PKT_TX_SLAB_DEFINE(http_cli_tx, 15);
NET_PKT_DATA_POOL_DEFINE(http_cli_data, 30);

#define SYS_LOG_DOMAIN "http-client"
#define NET_SYS_LOG_LEVEL SYS_LOG_LEVEL_DEBUG
#define NET_LOG_ENABLED 1


static struct k_mem_slab *tx_slab(void)
{
	return &http_cli_tx;
}

static struct net_buf_pool *data_pool(void)
{
	return &http_cli_data;
}
#else
#if defined(CONFIG_NET_L2_BT)
#error "TCP connections over Bluetooth need CONFIG_NET_CONTEXT_NET_PKT_POOL "\
	"defined."
#endif /* CONFIG_NET_L2_BT */

#define tx_slab NULL
#define data_pool NULL
#endif /* CONFIG_NET_CONTEXT_NET_PKT_POOL */

// bufsize for the reploy
#if !defined(RESULT_BUF_SIZE)
#define RESULT_BUF_SIZE 1600
#endif

static u8_t result[RESULT_BUF_SIZE];

// the same format lwm2m uses
struct float32_value {
    uint32_t val1; // whole number
    uint32_t val2; // 1/1e6
};

// description of the JSON that will be sent out
struct dummy_record {
    unsigned int timestamp;
    struct float32_value temperature;
    struct float32_value lat;
    struct float32_value lon;
};
static const struct json_obj_descr float32_descr[] = {
	JSON_OBJ_DESCR_PRIM(struct float32_value, val1, JSON_TOK_NUMBER),
	JSON_OBJ_DESCR_PRIM(struct float32_value, val2, JSON_TOK_NUMBER),
};
static const struct json_obj_descr dummy_record_descr[] = {
	JSON_OBJ_DESCR_PRIM(struct dummy_record, timestamp, JSON_TOK_NUMBER),
	JSON_OBJ_DESCR_OBJECT_NAMED(struct dummy_record, "temperature", temperature, float32_descr),
	JSON_OBJ_DESCR_OBJECT_NAMED(struct dummy_record, "lat", lat, float32_descr),
	JSON_OBJ_DESCR_OBJECT_NAMED(struct dummy_record, "lon", lon, float32_descr),
};

static int _get_test_json(const struct float32_value *temperature, uint8_t *buf, size_t bufsize)
{
    struct dummy_record tr = {
        .timestamp = k_uptime_get(),
        .temperature = {temperature->val1, temperature->val2},
        .lat = {1, 2},
        .lon = {-4, 5}
    };

	return json_obj_encode_buf(
            dummy_record_descr, ARRAY_SIZE(dummy_record_descr),
            &tr,
            buf, bufsize
            );
}


static int _send_data_http(const uint8_t *headers, const uint8_t *payload, size_t payload_size)
{
    ARG_UNUSED(payload_size);
	int ret = http_client_init(&http_ctx, SERVER_ADDR, SERVER_PORT, NULL,
			       K_FOREVER);
    if(ret < 0) {
        NET_ERR("Can't initialize http client");
        return -1;
    }
#if defined(CONFIG_NET_CONTEXT_NET_PKT_POOL)
	net_app_set_net_pkt_pool(&http_ctx.app_ctx, tx_slab, data_pool);
#endif

	http_set_cb(&http_ctx, NULL, http_received, NULL, NULL);
    ret = do_sync_http_req(
            &http_ctx, HTTP_POST, "/data",
            headers,
            "application/json", payload,
            result, RESULT_BUF_SIZE);
    if (ret < 0) {
        NET_ERR("Send failed: %d", ret);
        goto out;
    }

out:
    http_release(&http_ctx);

    return 0;
}

static int _send_record(const struct float32_value *temperature)
{
#define JSON_BUF_SIZE 128
    uint8_t buf[JSON_BUF_SIZE] = {0};
    uint8_t signature[65] = {0};
    privkey_t priv = {0};

    if(_get_test_json(temperature, buf, JSON_BUF_SIZE) < 0) {
        NET_ERR("get json failed");
        return -1;
    }
    if(eth_sign(priv.k, (uint8_t*)buf, strlen(buf), signature) < 0) {
        return -1;
    }
    if(bintohex_nonull(signature, 65, _sig_hdr.signature, sizeof(_sig_hdr.signature))) {
        return -1;
    }
    return _send_data_http((uint8_t*)&_sig_hdr, buf, strlen(buf));
}

static int upload_data(const struct shell *shell, int argc, char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    ARG_UNUSED(shell);

    struct float32_value temperature = {25, 0};

    if(argc > 1) {
        temperature.val1 = strtol(argv[1], NULL, 10);
        if(errno != 0) {
            printk("Invalid argument.\n");
            return 0;
        }
    }
    if(_send_record(&temperature) < 0) {
        printk("Error sending data\n");
        return 0;
    }
    return 0;

}

/*SHELL_CMD_REGISTER(upload, NULL, "http upload service", upload_data);*/



void upload_main(void)
{
    while(1) {
        struct float32_value temperature;
        temperature.val1 = 25;
        temperature.val2 = sys_rand32_get() % 1000000;
        _send_record(&temperature);
        k_sleep(1000);
    }
}
K_THREAD_DEFINE(upload_main_id, 4096, upload_main, NULL, NULL, NULL, 7, 0, K_NO_WAIT);

static int upload_stop(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    ARG_UNUSED(shell);
    k_thread_suspend(upload_main_id);
    return 0;
}

static int upload_start(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    ARG_UNUSED(shell);
    k_thread_resume(upload_main_id);
    return 0;
}

SHELL_CREATE_STATIC_SUBCMD_SET(sub_upload) {
    SHELL_CMD(start, NULL, "start upload service", upload_start),
    SHELL_CMD(stop, NULL, "stop upload service", upload_stop),
	SHELL_SUBCMD_SET_END
};
SHELL_CMD_REGISTER(upload, &sub_upload, "http upload service", NULL);
