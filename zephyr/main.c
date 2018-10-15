/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <shell/shell.h>

#include <eth/address.h>
#include <helpers/hextobin.h>
#include <time.h>
#include <string.h>

// #include "helpers/uint256.h"
//
#include "zephyr/wallet.h"

/* Change this if you have an LED connected to a custom port */
#ifndef LED0_GPIO_CONTROLLER
#define LED0_GPIO_CONTROLLER 	LED0_GPIO_PORT
#endif

#define LED_PORT LED0_GPIO_CONTROLLER

/* Change this if you have an LED connected to a custom pin */
#define LED	LED0_GPIO_PIN

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1000


static int shell_cmd_pk2addr(int argc, char *argv[])
{
    int ret;
    if(argc <= 1) {
        printk("usage: %s <hex_privkey>\n", argv[0]);
        return 0;
    }
    if(strlen(argv[1]) != 64) {
        printk("invalid private key: invalid length (64 chars expected)\n");
        return 0;
    }
    char pk[32] = {0};
    ret = hextobin(argv[1], pk, sizeof(pk));
    if(ret < 0) {
        printk("Invalid private key.\n");
        return 0;
    }

    char addr[20] = {0};
    ret = privkey_to_ethereum_address(pk, addr);
    if(ret < 0) {
        printk("Error while converting private key.\n");
        return 0;
    }
    for (size_t i = 0; i < sizeof(addr); i++) {
        printk("%02x", addr[i]);
    }
    printk("\n");

	return 0;
}

static struct shell_cmd commands[] = {
	{ "pk2addr", shell_cmd_pk2addr, NULL, NULL },
	{ NULL, NULL, NULL, NULL }
};

#define MY_SHELL_MODULE_CRYPTO "crypto_eth"
void main(void)
{
	int cnt = 0;
	struct device *dev;

    SHELL_REGISTER(MY_SHELL_MODULE_CRYPTO, commands);

	dev = device_get_binding(LED_PORT);
	/* Set LED pin as output */
	gpio_pin_configure(dev, LED, GPIO_DIR_OUT);

	while (1) {
		/* Set pin to HIGH/LOW every 1 second */
		gpio_pin_write(dev, LED, cnt % 2);
		cnt++;
		k_sleep(SLEEP_TIME);
	}
}
