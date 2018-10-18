/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define APP_REQ_TIMEOUT K_SECONDS(10)
#define APP_STARTUP_TIME K_SECONDS(20)

#if defined(CONFIG_HTTPS)
#define SERVER_PORT  4443
#else
#define SERVER_PORT  8000
#endif

#if defined(CONFIG_NET_IPV6)
#define SERVER_ADDR CONFIG_NET_CONFIG_PEER_IPV6_ADDR
#else
#define SERVER_ADDR CONFIG_NET_CONFIG_PEER_IPV4_ADDR
#endif

#define CLIENT_MANUFACTURER	    "AnyLedger"
#define CLIENT_MODEL_NUMBER	    "AnyLedger Wallet 0.1"
#define CLIENT_SERIAL_NUMBER	"0001"
#define CLIENT_FIRMWARE_VER	    "0.1"
#define CLIENT_DEVICE_TYPE	    "OMA-LWM2M Client"
#define CLIENT_HW_VER		    "0.0.1"
