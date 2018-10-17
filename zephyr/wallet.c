/**
* @brief 
* @file wallet.c
* @author J.H. 
* @date 2018-10-12
*/

/* system includes */
#include <zephyr.h>
#include <helpers/hextobin.h>
#include <string.h>
#include <stdlib.h>

/* local includes */
#include "wallet.h"
#include "zephyr/utils.h"

#include "eth/transaction.h"
#include "eth/address.h"

typedef struct {
    uint8_t k[32];
} privkey_t;
typedef uint32_t nonce_t;

typedef struct {
    privkey_t privkey;
    nonce_t nonce;
} account_t;

static account_t _account;

static int wallet_set_pk(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    int ret;
    if(argc <= 1) {
        printk_hex_nl(_account.privkey.k, sizeof(_account.privkey.k));
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
    memcpy(_account.privkey.k, pk, 32);

    return 0;
}

static int wallet_set_nonce(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    if(argc <= 1) {
        printk("%d\n", _account.nonce);
        return 0;
    }
    unsigned long nonce = strtol(argv[1], NULL, 10);
    if(errno != 0) {
        printk("Invalid nonce.\n");
        return 0;
    }
   _account.nonce = nonce;

    return 0;
}

static int wallet_tx(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    transaction_t tx;
    tx.nonce = _account.nonce;
    tx.gas_price = 1 * 1000000000;
    tx.gas_limit = 21000;
    tx_set_to(&tx, "2e83b5ae698e1f1ab5b6f4bb0732d72f0c74d049");
    tx_set_value_u64(&tx, 0);
    tx.data = NULL;
    tx.data_len = 0;

    #define BUFSIZE 256
    uint8_t buf[BUFSIZE];
    size_t txlen = tx_encode_sign(&tx, _account.privkey.k, buf, BUFSIZE);

    printk_hex_nl(buf, txlen);

    return 0;
}

static int wallet_pk2addr(const struct shell *shell, size_t argc, char *argv[])
{
    int ret;
    ARG_UNUSED(shell);
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

SHELL_CREATE_STATIC_SUBCMD_SET(sub_crypto) {
    SHELL_CMD(nonce, NULL, "nonce", wallet_set_nonce),
    SHELL_CMD(pk, NULL, "pk", wallet_set_pk),
    SHELL_CMD(pk2addr, NULL, "pk2addr", wallet_pk2addr),
    SHELL_CMD(tx, NULL, "tx", wallet_tx),
	SHELL_SUBCMD_SET_END
};
SHELL_CMD_REGISTER(crypto, &sub_crypto, "crypto eth", NULL);

