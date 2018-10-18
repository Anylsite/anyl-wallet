/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1000

/* Change this if you have an LED connected to a custom port */
#ifndef LED0_GPIO_CONTROLLER
#ifdef LED0_GPIO_PORT
#define LED0_GPIO_CONTROLLER 	LED0_GPIO_PORT
#else
#define LED0_GPIO_CONTROLLER "(fail)"
#define LED0_GPIO_PIN 0
#endif
#endif


/* Change this if you have an LED connected to a custom pin */
#define LED	LED0_GPIO_PIN
#define LED_PORT LED0_GPIO_CONTROLLER

static struct device *init_led()
{
    int ret;
	struct device *led_dev;
	led_dev = device_get_binding(LED_PORT);
    if(led_dev == NULL) {
        return NULL;
    }
	/* Set LED pin as output */
	ret = gpio_pin_configure(led_dev, LED, GPIO_DIR_OUT);
	if (ret) {
		return NULL;
	}
    return led_dev;
}

void main(void)
{
	int cnt = 0;
    struct device *dev = init_led();

	while (1) {
		/* Set pin to HIGH/LOW every 1 second */
        if(dev != NULL) {
            gpio_pin_write(dev, LED, cnt % 2);
            cnt++;
        }
		k_sleep(SLEEP_TIME);
	}
}
