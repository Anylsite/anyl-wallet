/**
* @brief 
* @file wallet.c
* @author J.H. 
* @date 2018-10-12
*/

/* system includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <zephyr.h>
#include <shell/shell.h>
#pragma GCC diagnostic pop
#include <string.h>
#include <stdlib.h>

/* local includes */
#include "wallet.h"
#include "helpers/hextobin.h"
#include "zephyr-wallet/utils.h"
#include "zephyr-wallet/web3_rpc.h"

#include "eth/transaction.h"
#include "eth/address.h"

static account_t _account = {
    .privkey = {{0}},
    .nonce = 0,
    .gas_price = 1e9,
    .gas_limit = 21000
};

void wallet_set_global_privkey(const privkey_t *pk)
{
    _account.privkey = *pk;
    privkey_to_ethereum_address(_account.privkey.k, &_account.address);
}

account_t *wallet_get_account()
{
    return &_account;
}

static int wallet_set_pk(const struct shell *shell, size_t argc, char *argv[])
{
    int ret;
    if(argc <= 1) {
        printk_hex_nl(shell, _account.privkey.k, sizeof(_account.privkey.k));
        return 0;
    }
    if(strlen(argv[1]) != 64) {
        printk("invalid private key: invalid length (64 chars expected)\n");
        return 0;
    }
    privkey_t pk = {0};
    ret = hextobin(argv[1], (uint8_t*)&pk, sizeof(pk));
    if(ret < 0) {
        printk("Invalid private key.\n");
        return 0;
    }
    wallet_set_global_privkey(&pk);

    return 0;
}

static int __convert_ulong(const char *str, const char *name, uint32_t *output)
{
    if(str[0] == '-') {
        printk("invalid argument: %s\n", name);
        return -1;
    }
    char *endptr = NULL;
    unsigned long val = strtoul(str, &endptr, 10);
    if((errno != 0) || ((endptr != NULL) && (*endptr != 0))) {
        printk("invalid argument: %s\n", name);
        return -1;
    }
    *output = val;
    return 0;
}

static int wallet_set_nonce(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    if(argc <= 1) {
        printk("%u\n", _account.nonce);
        return 0;
    }
    return __convert_ulong(argv[1], "set_nonce", &_account.nonce);
}

static int wallet_set_gas_price(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    if(argc <= 1) {
        printk("%u\n", _account.gas_price);
        return 0;
    }
    return __convert_ulong(argv[1], "gas_price", &_account.gas_price);
}

static int wallet_set_gas_limit(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    if(argc <= 1) {
        printk("%u\n", _account.gas_limit);
        return 0;
    }
    return __convert_ulong(argv[1], "gas_limit", &_account.gas_limit);
}

static int wallet_transfer(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    transaction_t tx;
    // first parameter is recipient address
    if(argc < 2) {
        printk("missing argument: address\n");
        return -1;
    }
    if(hextobin(argv[1], (uint8_t*)&tx.to, sizeof(tx.to)) < 0) {
        printk("invalid argument: address\n");
        return -1;
    }
    // amount to send
    if(argc < 3) {
        printk("missing argument: transfer amount\n");
        return -1;
    }
    char *endptr = NULL;
    uint64_t tx_value = strtoul(argv[2], &endptr, 10);
    if((errno != 0) || ((endptr != NULL) && (*endptr != 0))) {
        printk("invalid argument: amount\n");
        return -1;
    }
    // build the transaction
    tx.nonce = _account.nonce;
    tx.gas_price = 1 * 1000000000;
    tx.gas_limit = 21000;
    tx_set_value_u64(&tx, tx_value);
    tx.data = NULL;
    tx.data_len = 0;

    #define BUFSIZE 256
    uint8_t buf[BUFSIZE];
    size_t txlen = tx_encode_sign(&tx, _account.privkey.k, buf, BUFSIZE);

    uint256_t tx_hash;
    if(web3_eth_sendRawTransaction(buf, txlen, &tx_hash) < 0) {
        printk("Error: eth_sendRawTransaction()\n");
        return -1;
    }
    printk_uint256(shell, &tx_hash);

    return 0;
}

static int wallet_pk2addr(const struct shell *shell, size_t argc, char *argv[])
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

    address_t addr;
    ret = privkey_to_ethereum_address(pk, &addr);
    if(ret < 0) {
        printk("Error while converting private key.\n");
        return 0;
    }
    printk_hex_nl(shell, addr.a, sizeof(addr.a));

	return 0;
}

static int wallet_sync(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    uint256_t tx_count;
    if(web3_eth_getTransactionCount(&_account.address, &tx_count) < 0) {
        printk("Error: eth_getTransactionCount failed\n");
        return 0;
    }
    _account.nonce = LOWER(LOWER(tx_count));
    return 0;
}

static int wallet_balance(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(argv);
    ARG_UNUSED(argc);
    uint256_t out = {0};
    uint256_t decimals;
    uint256_t div, mod;
    clear256(&decimals);
    clear256(&div);
    clear256(&mod);
    LOWER(LOWER(decimals)) = 1000000000000000000;
    if(web3_eth_getBalance(&_account.address, &out) < 0) {
        printk("Error: eth_getBalance failed");
        return -1;
    }
    divmod256(&out, &decimals, &div, &mod);
    printk_uint256_int(shell, &div, 0, FMT_EMPTY);
    printk(".");
    printk_uint256_int(shell, &mod, 18, FMT_FIXED | FMT_NO_TRAIL_ZERO);
    printk("\n");
    return 0;
}

SHELL_CREATE_STATIC_SUBCMD_SET(sub_wallet) {
    SHELL_CMD(nonce, NULL, "nonce", wallet_set_nonce),
    SHELL_CMD(gas_price, NULL, "gas price", wallet_set_gas_price),
    SHELL_CMD(gas_limit, NULL, "gas limit", wallet_set_gas_limit),
    SHELL_CMD(pk, NULL, "private key", wallet_set_pk),
    SHELL_CMD(pk2addr, NULL, "convert private key to an address", wallet_pk2addr),
    SHELL_CMD(transfer, NULL, "transfer ETH", wallet_transfer),
    SHELL_CMD(sync, NULL, "sync account nonce", wallet_sync),
    SHELL_CMD(balance, NULL, "account balance", wallet_balance),
	SHELL_SUBCMD_SET_END
};
SHELL_CMD_REGISTER(wallet, &sub_wallet, "crypto eth", NULL);
