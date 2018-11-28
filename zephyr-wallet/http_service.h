#ifndef _HTTP_SERVICE_H_
#define _HTTP_SERVICE_H_
/* system includes */
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct {
    const char      *host;
    uint16_t        port;
    const char      *url;
    const char      *content_type;
    const uint8_t   *headers;
    const uint8_t   *payload;
    size_t          payload_size;
} http_nfo_t;

int http_send_data(const http_nfo_t *nfo, uint8_t **body, size_t *content_len);

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_SERVICE_H_ */

