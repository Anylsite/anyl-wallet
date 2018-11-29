/**
* @brief eth node connection
* @file http_service.c
* @author J.H. 
* @date 2018-10-23
*/

/* system includes */
#define LOG_MODULE_NAME http_service
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

typedef struct {
    struct k_mutex  *mtx;
} http_nfo_priv_t;
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
    assert(nfo->_priv == NULL);
}

// by default http client skips body of E500 responses. We want to keep it 
//  because Sawtooth sends error message in it
static int on_headers_complete(struct http_parser *parser)
{
	struct http_ctx *ctx = CONTAINER_OF(parser,
					    struct http_ctx,
					    http.parser);

	if ((ctx->http.req.method == HTTP_HEAD ||
	     ctx->http.req.method == HTTP_OPTIONS)
	    && ctx->http.rsp.content_length > 0) {
		return 1;
	}

	if ((ctx->http.req.method == HTTP_PUT ||
	     ctx->http.req.method == HTTP_POST)
	    && ctx->http.rsp.content_length == 0) {
		return 1;
	}


	return 0;
}

int http_send_data(http_nfo_t *nfo)
{
    k_mutex_lock(&http_client_mutex, K_FOREVER);
    __assert_nfo(nfo);
    nfo->_priv = NULL;

	int ret = http_client_init(&http_ctx, nfo->host, nfo->port, NULL,
			       K_FOREVER);
	http_ctx.http.parser_settings.on_headers_complete = on_headers_complete;
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

    nfo->content = http_ctx.http.rsp.body_start;
    nfo->content_size = http_ctx.http.rsp.content_length;
    nfo->_priv = &http_client_mutex;
    ret = http_ctx.http.parser.status_code;
    assert(ret > 0);
out:
    // in case of error the mutex can be released - no shared data access is possible
    if(ret < 0) {
        k_mutex_unlock(&http_client_mutex);
    }
    http_release(&http_ctx);

    return ret;
}

void http_nfo_free(http_nfo_t *nfo)
{
    if(nfo->_priv != NULL) {
        nfo->content = NULL;
        nfo->content_size = 0;
        k_mutex_unlock((struct k_mutex*)nfo->_priv);
    }
}
