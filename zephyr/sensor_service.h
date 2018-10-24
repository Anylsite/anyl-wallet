#ifndef _SENSOR_SERVICE_H_
#define _SENSOR_SERVICE_H_
/* system includes */
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

int get_sensor_data(int32_t *temperature, int32_t *humidity);

#ifdef __cplusplus
}
#endif

#endif /* _SENSOR_SERVICE_H_ */

