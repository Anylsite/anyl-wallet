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

/* local includes */
#include "wallet.h"
#include "eth/account.h"
#include "zephyr/utils.h"

#include "eth/transaction.h"

static account_t _account;

int wallet_set_pk(int argc, char *argv[])
{
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

int wallet_set_nonce(int argc, char *argv[])
{
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

int wallet_tx(int argc, char *argv[])
{
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

static struct shell_cmd wallet_commands[] = {
	{ "pk", wallet_set_pk, NULL, NULL },
	{ "nonce", wallet_set_nonce, NULL, NULL },
	{ "tx", wallet_tx, NULL, NULL },
	{ NULL, NULL, NULL, NULL }
};
SHELL_REGISTER("wallet", wallet_commands);
