/**
* @brief run a thread that reads temp & humidity from the sensor and store the value in a global variable
* @file sensor_service.c
* @author J.H. 
* @date 2018-10-24
*/

/* system includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <zephyr.h>
#include <sensor.h>
#include <shell/shell.h>
#pragma GCC diagnostic pop

/* local includes */
#include "sensor_service.h"
#include "compat.h"

static struct sensor_value _temp = {0,0};
static struct sensor_value _humidity = {0,0};
static int _last_err = 0;


#define SLEEP_TIME  1000

int get_sensor_data(int32_t *temperature, int32_t *humidity)
{
    // TODO maybe add a mutex here
    *temperature = _temp.val1;
    *humidity = _humidity.val1;
    return _last_err;
}

#ifdef CONFIG_SENSOR
void temperature_main(void)
{
	struct device *dev = device_get_binding("DHT11");
    while(1) {
		_last_err = sensor_sample_fetch(dev);
        if(_last_err == 0) {
            sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &_temp);
            sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &_humidity);
        }
		k_sleep(SLEEP_TIME);
    }
}
#else
void temperature_main(void)
{
    while(1) {
		_last_err = 0;
        _temp.val1 = 20 + (rand() % 2);
        _humidity.val1 = 40 + (rand() % 3);
		k_sleep(SLEEP_TIME * 5);
    }
}
#endif


// start the temperature polling thread
K_THREAD_DEFINE(_temperature_main_id, 1024, temperature_main, NULL, NULL, NULL, 7, 0, K_NO_WAIT);

static int sensor_get(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    ARG_UNUSED(shell);

    if(_last_err < 0) {
        printk("error getting data from the sensor (%d)", _last_err);
        return 0;
    }
    printk("%d C %d%%\n", _temp.val1, _humidity.val1);
    return 0;
}

SHELL_CREATE_STATIC_SUBCMD_SET(sub_sensor) {
    SHELL_CMD(get, NULL, "get data from the sensor", sensor_get),
	SHELL_SUBCMD_SET_END
};
SHELL_CMD_REGISTER(sensor, &sub_sensor, "temperature sensor", NULL);

void sensor_shell_register()
{}
