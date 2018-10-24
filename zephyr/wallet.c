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
#include <shell/shell.h>

/* local includes */
#include "wallet.h"
#include "zephyr/utils.h"
#include "zephyr/web3_rpc.h"

#include "eth/transaction.h"
#include "eth/address.h"

static account_t _account;

void wallet_set_global_privkey(const privkey_t *pk)
{
    _account.privkey = *pk;
}

account_t *wallet_get_account()
{
    return &_account;
}

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

static int wallet_transfer(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    transaction_t tx;
    tx.nonce = _account.nonce;
    tx.gas_price = 1 * 1000000000;
    tx.gas_limit = 21000;
    if(argc < 2) {
        printk("missing argument: address\n");
        return 0;
    }
    if(hextobin(argv[1], (uint8_t*)&tx.to, sizeof(tx.to)) < 0) {
        printk("invalid argument: address\n");
        return 0;
    }
    tx_set_value_u64(&tx, 0);
    tx.data = NULL;
    tx.data_len = 0;

    #define BUFSIZE 256
    uint8_t buf[BUFSIZE];
    size_t txlen = tx_encode_sign(&tx, _account.privkey.k, buf, BUFSIZE);

    if(web3_eth_sendRawTransaction(buf, txlen) < 0) {
        printk("Error: eth_sendRawTransaction()");
        return -1;
    }

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

    address_t addr;
    ret = privkey_to_ethereum_address(pk, &addr);
    if(ret < 0) {
        printk("Error while converting private key.\n");
        return 0;
    }
    printk_hex_nl(addr, sizeof(addr));

	return 0;
}

static int wallet_sync(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    address_t addr;
    int ret = privkey_to_ethereum_address(_account.privkey.k, &addr);
    if(ret < 0) {
        printk("Error while converting private key.\n");
        return 0;
    }
    uint256_t tx_count;
    if(web3_eth_getTransactionCount(&addr, &tx_count) < 0) {
        printk("Error: eth_getTransactionCount failed\n");
        return 0;
    }
    _account.nonce = LOWER(LOWER(tx_count));
    return 0;
}

SHELL_CREATE_STATIC_SUBCMD_SET(sub_wallet) {
    SHELL_CMD(nonce, NULL, "nonce", wallet_set_nonce),
    SHELL_CMD(pk, NULL, "pk", wallet_set_pk),
    SHELL_CMD(pk2addr, NULL, "pk2addr", wallet_pk2addr),
    SHELL_CMD(transfer, NULL, "transfer", wallet_transfer),
    SHELL_CMD(sync, NULL, "sync", wallet_sync),
	SHELL_SUBCMD_SET_END
};
SHELL_CMD_REGISTER(wallet, &sub_wallet, "crypto eth", NULL);

