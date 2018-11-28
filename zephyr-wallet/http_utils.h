#ifndef _HTTP_UTILS_H_
#define _HTTP_UTILS_H_
/* system includes */
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif
void http_received(struct http_ctx *ctx,
			  struct net_pkt *pkt,
			  int status,
			  u32_t flags,
			  const struct sockaddr *dst,
			  void *user_data);
int do_sync_http_req(struct http_ctx *ctx,
			    enum http_method method,
			    const char  *url,
			    const char  *headers,
			    const char  *content_type,
			    const char  *payload,
                size_t      payload_size,
                uint8_t     *result,
                size_t      result_size
                );

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_UTILS_H_ */

