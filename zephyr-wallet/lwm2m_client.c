/**
* @brief 
* @file lwm2m_client.c
* @author J.H. 
* @date 2018-10-18
*/

// build this only if LWM2M_CLIENT is configured
#include <autoconf.h>
#ifdef CONFIG_LWM2M
/* system includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#include <zephyr.h>
#include <net/lwm2m.h>
/*#include <board.h>*/
#include <gpio.h>
#pragma GCC diagnostic pop

/* local includes */
#include "lwm2m_client.h"
#include "zephyr-wallet/config.h"
#include "zephyr-wallet/sensor_service.h"

static struct lwm2m_ctx client;
#define WAIT_TIME	    K_SECONDS(10)
#define CONNECT_TIME	K_SECONDS(10)
#define SLEEP_TIME      K_SECONDS(5)

static int lwm2m_setup(void)
{
	lwm2m_engine_set_res_data("3/0/0", CLIENT_MANUFACTURER,
				  sizeof(CLIENT_MANUFACTURER),
				  LWM2M_RES_DATA_FLAG_RO);
	lwm2m_engine_set_res_data("3/0/1", CLIENT_MODEL_NUMBER,
				  sizeof(CLIENT_MODEL_NUMBER),
				  LWM2M_RES_DATA_FLAG_RO);
	lwm2m_engine_set_res_data("3/0/2", CLIENT_SERIAL_NUMBER,
				  sizeof(CLIENT_SERIAL_NUMBER),
				  LWM2M_RES_DATA_FLAG_RO);
	lwm2m_engine_set_res_data("3/0/3", CLIENT_FIRMWARE_VER,
				  sizeof(CLIENT_FIRMWARE_VER),
				  LWM2M_RES_DATA_FLAG_RO);
	lwm2m_engine_set_res_data("3/0/17", CLIENT_DEVICE_TYPE,
				  sizeof(CLIENT_DEVICE_TYPE),
				  LWM2M_RES_DATA_FLAG_RO);
	lwm2m_engine_set_res_data("3/0/18", CLIENT_HW_VER,
				  sizeof(CLIENT_HW_VER),
				  LWM2M_RES_DATA_FLAG_RO);

	/* setup TEMP SENSOR object */
	lwm2m_engine_create_obj_inst("3303/0");


	return 0;
}

static void rd_client_event(struct lwm2m_ctx *client,
			    enum lwm2m_rd_client_event client_event)
{
    ARG_UNUSED(client);
	switch (client_event) {

	case LWM2M_RD_CLIENT_EVENT_NONE:
		/* do nothing */
		break;

	case LWM2M_RD_CLIENT_EVENT_BOOTSTRAP_FAILURE:
		LOG_DBG("Bootstrap failure!");
		break;

	case LWM2M_RD_CLIENT_EVENT_BOOTSTRAP_COMPLETE:
		LOG_DBG("Bootstrap complete");
		break;

	case LWM2M_RD_CLIENT_EVENT_REGISTRATION_FAILURE:
		LOG_DBG("Registration failure!");
		break;

	case LWM2M_RD_CLIENT_EVENT_REGISTRATION_COMPLETE:
		LOG_DBG("Registration complete");
		break;

	case LWM2M_RD_CLIENT_EVENT_REG_UPDATE_FAILURE:
		LOG_DBG("Registration update failure!");
		break;

	case LWM2M_RD_CLIENT_EVENT_REG_UPDATE_COMPLETE:
		LOG_DBG("Registration update complete");
		break;

	case LWM2M_RD_CLIENT_EVENT_DEREGISTER_FAILURE:
		LOG_DBG("Deregister failure!");
		break;

	case LWM2M_RD_CLIENT_EVENT_DISCONNECT:
		LOG_DBG("Disconnected");
		break;

	}
}

int lwm2m_init()
{
	int ret = lwm2m_setup();
	if (ret < 0) {
		LOG_ERR("Cannot setup LWM2M fields (%d)", ret);
		return -1;
	}
	(void)memset(&client, 0x0, sizeof(client));
	client.net_init_timeout = WAIT_TIME;
	client.net_timeout = CONNECT_TIME;
#if defined(CONFIG_NET_CONTEXT_NET_PKT_POOL)
	client.tx_slab = tx_udp_slab;
	client.data_pool = data_udp_pool;
#endif
#if defined(CONFIG_NET_IPV6)
	ret = lwm2m_rd_client_start(&client, CONFIG_NET_CONFIG_PEER_IPV6_ADDR,
				    CONFIG_LWM2M_PEER_PORT, CONFIG_BOARD,
				    rd_client_event);
#elif defined(CONFIG_NET_IPV4)
	ret = lwm2m_rd_client_start(&client, CONFIG_NET_CONFIG_PEER_IPV4_ADDR,
				    CONFIG_LWM2M_PEER_PORT, CONFIG_BOARD,
				    rd_client_event);
#else
	LOG_ERR("LwM2M client requires IPv4 or IPv6.");
	ret = -EPROTONOSUPPORT;
#endif
	if (ret < 0) {
		LOG_ERR("LWM2M init LWM2M RD client error (%d)", ret);
		return ret;
	}

    return 0;
}


static void lwm2m_main(void)
{
	struct float32_value float_temp = {0,0}, float_humidity = {0,0};
    while(1) {
        get_sensor_data(&float_temp.val1, &float_humidity.val1);
        lwm2m_engine_set_float32("3303/0/5700", &float_temp);
        // humidity object is not supported yet
/*        lwm2m_engine_set_float32("3304/0/5700", &float_humidity);*/
		k_sleep(SLEEP_TIME);
    }
}

K_THREAD_DEFINE(_lwm2m_id, 1024, lwm2m_main, NULL, NULL, NULL, 7, 0, K_NO_WAIT);
#endif
