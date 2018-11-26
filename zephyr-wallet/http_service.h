#ifndef _HTTP_SERVICE_H_
#define _HTTP_SERVICE_H_
/* system includes */
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

int http_send_data(const uint8_t *headers, const uint8_t *payload, size_t payload_size, uint8_t **body, size_t *content_len);

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_SERVICE_H_ */

