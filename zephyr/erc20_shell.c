/**
* @brief 
* @file erc20_shell.c
* @author J.H. 
* @date 2018-10-31
*/

/* system includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <shell/shell.h>
#pragma GCC diagnostic pop
#include <stdlib.h>

/* local includes */
#include "erc20_shell.h"
#include "helpers/uint256.h"
#include "helpers/hextobin.h"
#include "wallet.h"
#include "eth/transaction.h"
#include "eth/address.h"
#include "eth/data.h"
#include "eth/web3.h"
#include "eth/web3_jsonp.h"
#include "erc20_abi.h"
#include "zephyr/utils.h"
#include "zephyr/web3_rpc.h"

static int erc20_balance(const struct shell *shell, size_t argc, char *argv[])
{
    transaction_t tx;
    memset(&tx, 0, sizeof(tx));
    account_t *acc = wallet_get_account();
    assert(acc != NULL);
    // first parameter is recipient address
    if(argc < 2) {
        printk("missing argument: address\n");
        return -1;
    }
    if(hextobin(argv[1], (uint8_t*)&tx.to, sizeof(tx.to)) < 0) {
        printk("invalid argument: address\n");
        return -1;
    }
    // build the transaction

    data_block_t tx_data;
    uint8_t datablk[256+4];
    tx_data.data = datablk;
    tx_data.data_len = sizeof(datablk);
    int encoded_data_len = Token_balanceOf(&acc->address, &tx_data);
    if(encoded_data_len < 0) {
        shell_fprintf(shell, SHELL_WARNING, "Can't encode Token::balanceOf()\n");
        return -1;
    }
    tx.data = datablk;
    tx.data_len = encoded_data_len;

    uint256_t out;
    if(web3_eth_call(&acc->address, &tx, &out, TX_NO_FROM | TX_NO_GAS | TX_NO_GASPRICE | TX_NO_VALUE) < 0) {
        return -1;
    }

    printk_uint256(shell, &out);

    return 0;
}

static int erc20_transfer(const struct shell *shell, size_t argc, char *argv[])
{
    transaction_t tx;
    account_t *acc = wallet_get_account();
    assert(acc != NULL);
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
    tx.nonce = acc->nonce;
    tx.gas_price = 1 * 1000000000;
    tx.gas_limit = 31843;
    clear256(&tx.value);
    uint256_t tx_value_256;
    clear256(&tx_value_256);
    LOWER(LOWER(tx_value_256)) = tx_value;

    data_block_t tx_data;
    uint8_t datablk[256+4];
    tx_data.data = datablk;
    tx_data.data_len = sizeof(datablk);
    int encoded_data_len = Token_transfer(&acc->address, &tx_value_256, &tx_data);
    if(encoded_data_len < 0) {
        shell_fprintf(shell, SHELL_WARNING, "Can't encode Token::transfer()\n");
        return -1;
    }
    tx.data = datablk;
    tx.data_len = encoded_data_len;

    #define BUFSIZE 256
    uint8_t buf[BUFSIZE];
    size_t txlen = tx_encode_sign(&tx, acc->privkey.k, buf, BUFSIZE);

    uint256_t tx_hash;
    if(web3_eth_sendRawTransaction(buf, txlen, &tx_hash) < 0) {
        printk("Error: eth_sendRawTransaction()\n");
        return -1;
    }
    printk_uint256(shell, &tx_hash);

    return 0;
}

SHELL_CREATE_STATIC_SUBCMD_SET(sub_erc20) {
    SHELL_CMD(balance, NULL, "balance", erc20_balance),
    SHELL_CMD(transfer, NULL, "transfer", erc20_transfer),
	SHELL_SUBCMD_SET_END
};
SHELL_CMD_REGISTER(erc20, &sub_erc20, "erc20 utils", NULL);
