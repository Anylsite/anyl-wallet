/*
 * Copyright (c) 2018 AnyLedger
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* system includes */
#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <string.h>

/* local includes */
#include "zephyr-wallet/wallet.h"
#include "zephyr-wallet/shell_modules.h"

#include "zephyr-wallet/lwm2m_client.h"
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

extern uint8_t g_zephyr_private_key[32];

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
/*    lwm2m_init();*/
    privkey_t pk;
    memcpy(&pk.k, g_zephyr_private_key, 32);

    wallet_set_global_privkey(&pk);
    wallet_register_shell_modules();

	while (1) {
		/* Set pin to HIGH/LOW every 1 second */
        if(dev != NULL) {
            gpio_pin_write(dev, LED, cnt % 2);
            cnt++;
        }
		k_sleep(SLEEP_TIME);
	}
}
