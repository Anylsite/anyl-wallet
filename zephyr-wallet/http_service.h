#ifndef _HTTP_SERVICE_H_
#define _HTTP_SERVICE_H_
/* system includes */
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

// this module provides a simple http client that can be shared by multiple threads
//
// how to use:
// 1) set http_nfo_t structure
// 2) call http_send_data()
// 3) if there's a reply it will be accessible via char **body item of the http_nfo_t
// 4) release the client buffer by using http_nfo_free(). The client is then available to be used from
//      another thread

typedef struct {
    const char      *host;          // host to connect to
    uint16_t        port;           // destination port
    const char      *url;           // resource
    const char      *content_type;  // content-type header value
    const uint8_t   *headers;       // other headers (or NULL)
    const uint8_t   *payload;       // payload to send
    size_t          payload_size;   // size of the payload (in bytes)
    char            *content;       // content of the reply, or NULL in case of error
    size_t          content_size;   // size of the body
    void            *_priv;         // client mutex pointer. don't mess with this
} http_nfo_t;

int http_send_data(http_nfo_t *nfo);
void http_nfo_free(http_nfo_t *nfo);

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_SERVICE_H_ */

