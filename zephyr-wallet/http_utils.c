/**
* @brief 
* @file http_utils.c
* @author J.H. 
* @date 2018-10-16
*/
#define LOG_MODULE_NAME net_http_utils
#define NET_LOG_LEVEL LOG_LEVEL_WRN

/* system includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#include <net/net_app.h>
#include <net/http.h>
#pragma GCC diagnostic pop

/* local includes */
#include "http_utils.h"
#include "config.h"

void http_received(struct http_ctx *ctx,
			  struct net_pkt *pkt,
			  int status,
			  u32_t flags,
			  const struct sockaddr *dst,
			  void *user_data)
{
    ARG_UNUSED(ctx);
    ARG_UNUSED(flags);
    ARG_UNUSED(dst);
    ARG_UNUSED(user_data);

	if (!status) {
		if (pkt) {
			NET_DBG("Received %d bytes data",
				net_pkt_appdatalen(pkt));
			net_pkt_unref(pkt);
		}
	} else {
		NET_ERR("Receive error (%d)", status);

		if (pkt) {
			net_pkt_unref(pkt);
		}
	}
}

int do_sync_http_req(struct http_ctx *ctx,
			    enum http_method method,
			    const char *url,
			    const char *headers,
			    const char *content_type,
			    const char *payload,
                size_t      payload_size,
                uint8_t *result,
                size_t result_size)
{
	struct http_request req = {};
	int ret;

	req.method = method;
	req.url = url;
	req.protocol = " " HTTP_PROTOCOL;
    req.header_fields = headers;
    req.content_type_value = content_type;
    req.payload = payload;
    req.payload_size = payload_size;

	ret = http_client_send_req(ctx, &req, NULL, result, result_size,
				   NULL, APP_REQ_TIMEOUT);
	if (ret < 0) {
		NET_ERR("Cannot send %s request (%d)", http_method_str(method),
			ret);
		goto out;
	}

	if (ctx->http.rsp.data_len > result_size) {
		NET_ERR("Result buffer overflow by %zd bytes",
		       ctx->http.rsp.data_len - result_size);

		ret = -E2BIG;
	} else {
		NET_INFO("HTTP server response status: %s",
			 ctx->http.rsp.http_status);

		if (ctx->http.parser.http_errno) {
			if (method == HTTP_OPTIONS) {
				/* Ignore error if OPTIONS is not found */
				goto out;
			}

			NET_INFO("HTTP parser status: %s",
			       http_errno_description(
				       ctx->http.parser.http_errno));
			ret = -EINVAL;
			goto out;
		}

		if (method != HTTP_HEAD) {
			if (ctx->http.rsp.body_found) {
				NET_INFO("HTTP body: %zd bytes, "
					 "expected: %zd bytes",
					 ctx->http.rsp.processed,
					 ctx->http.rsp.content_length);
			} else {
				NET_ERR("Error detected during HTTP msg "
					"processing");
			}
		}
	}

out:
	http_close(ctx);

	return ret;
}
