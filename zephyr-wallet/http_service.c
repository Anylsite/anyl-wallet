/**
* @brief eth node connection
* @file http_service.c
* @author J.H. 
* @date 2018-10-23
*/

/* system includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#include <zephyr.h>
#include <net/http.h>
#pragma GCC diagnostic pop
#include <stdint.h>

/* local includes */
#include "http_service.h"
#include "http_utils.h"

// HTTP
static struct http_ctx http_ctx;
// bufsize for the reply
#if !defined(RESULT_BUF_SIZE)
#define RESULT_BUF_SIZE 1600
#endif

// mutex to prevent reentrancy & concurrent access
K_MUTEX_DEFINE(http_client_mutex);

// WEB3
static u8_t result[RESULT_BUF_SIZE];

static void __assert_nfo(const http_nfo_t *nfo)
{
    assert(nfo->host != NULL);
    assert(nfo->port != NULL);
    assert(nfo->url != NULL);
    assert(nfo->content_type != NULL);
}

int http_send_data(const http_nfo_t *nfo, uint8_t **body, size_t *content_len)
{
    k_mutex_lock(&http_client_mutex, K_FOREVER);
    __assert_nfo(nfo);
	int ret = http_client_init(&http_ctx, nfo->host, nfo->port, NULL,
			       K_FOREVER);
    if(ret < 0) {
        NET_ERR("Can't initialize http client");
        goto out;
    }
#if defined(CONFIG_NET_CONTEXT_NET_PKT_POOL)
	net_app_set_net_pkt_pool(&http_ctx.app_ctx, tx_slab, data_pool);
#endif

	http_set_cb(&http_ctx, NULL, http_received, NULL, NULL);
    ret = do_sync_http_req(
            &http_ctx, HTTP_POST, nfo->url,
            nfo->headers,
            nfo->content_type,
            nfo->payload, nfo->payload_size,
            result, RESULT_BUF_SIZE);
    if (ret < 0) {
        NET_ERR("Send failed: %d", ret);
        goto out;
    }
    *body = http_ctx.http.rsp.body_start;
    *content_len = http_ctx.http.rsp.content_length;

out:
    http_release(&http_ctx);
    k_mutex_unlock(&http_client_mutex);

    return ret;
}
