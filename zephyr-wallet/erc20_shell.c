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
#include "endian.h"
#include "helpers/math.h"
#include "helpers/uint256.h"
#include "helpers/hextobin.h"
#include "eth/transaction.h"
#include "eth/address.h"
#include "eth/data.h"
#include "eth/web3.h"
#include "eth/web3_jsonp.h"
#include "erc20_abi.h"
#include "zephyr-wallet/utils.h"
#include "zephyr-wallet/web3_rpc.h"
#include "zephyr-wallet/erc20_shell.h"
#include "zephyr-wallet/wallet.h"

#define TOKEN_SYMBOL_MAX    3
#define TOKENS_MAX          5

typedef struct {
    address_t   address;
    uint8_t     decimals;
    char        symbol[TOKEN_SYMBOL_MAX+1];
    uint8_t     used;
} token_nfo_t;

static token_nfo_t _tokens[TOKENS_MAX];
uint8_t _token_idx = 0;

static int _token_add(const address_t *addr, uint8_t decimals, char *symbol)
{
    assert(addr != NULL);
    assert(symbol != NULL);
    uint8_t tkn_idx = 0;
    for(tkn_idx = 0; tkn_idx < TOKENS_MAX; tkn_idx++) {
        if(_tokens[tkn_idx].used == 1) { continue; }
        _tokens[tkn_idx].decimals = decimals;
        memcpy(&_tokens[tkn_idx].address, addr, sizeof(address_t));
        strncpy(_tokens[tkn_idx].symbol, symbol, TOKEN_SYMBOL_MAX);
        _tokens[tkn_idx].symbol[TOKEN_SYMBOL_MAX] = '\0';
        _tokens[tkn_idx].used = 1;
        return 0;
    }
    return -1;
}

static int _get_token_by_index(const struct shell *shell, const char *arg, token_nfo_t **out)
{
    errno = 0;
    char *endptr = NULL;
    uint8_t token_idx = strtoul(arg, &endptr, 10);
    if((errno != 0) || ((endptr != NULL) && (*endptr != 0)) || (token_idx >= TOKENS_MAX)) {
        shell_fprintf(shell, SHELL_WARNING, "invalid token index\n");
        return -1;
    }

    *out = &_tokens[token_idx];
    if((*out)->used == 0) {
        shell_fprintf(shell, SHELL_WARNING, "invalid token index\n");
        return -1;
    }
    return 0;
}

static int erc20_balance(const struct shell *shell, size_t argc, char *argv[])
{
    transaction_t tx;
    memset(&tx, 0, sizeof(tx));
    account_t *acc = wallet_get_account();
    assert(acc != NULL);
    if(argc != 2) {
        shell_fprintf(shell, SHELL_WARNING, "use: erc20 balance <token_idx>\n");
        return -1;
    }
    // first parameter is a token index
    token_nfo_t *tkn_p = NULL;
    if(_get_token_by_index(shell, argv[1], &tkn_p) != 0) {
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
    tx.to = tkn_p->address;

    uint256_t out;
    if(web3_eth_call(&acc->address, &tx, &out, TX_NO_FROM | TX_NO_GAS | TX_NO_GASPRICE | TX_NO_VALUE) < 0) {
        return -1;
    }
    shell_print_decimal_u256(shell, &out, tkn_p->decimals);

    return 0;
}

static int erc20_transfer(const struct shell *shell, size_t argc, char *argv[])
{
    transaction_t tx;
    account_t *acc = wallet_get_account();
    assert(acc != NULL);
    if(argc != 4) {
        shell_fprintf(shell, SHELL_WARNING, "use: erc20 transfer <index> <address_to> <amount>\n");
        return -1;
    }
    // first parameter is an index
    token_nfo_t *tkn_p = NULL;
    if(_get_token_by_index(shell, argv[1], &tkn_p) != 0) {
        return -1;
    }

    // second parameter is recipient address
    address_t address_to;
    if(hextobin(argv[2], (uint8_t*)&address_to, sizeof(address_to)) < 0) {
        shell_fprintf(shell, SHELL_NORMAL, "invalid argument: address\n");
        return -1;
    }
    // third parameter is amount to transfer
    uint64_t tx_value = 0;
    if(shell_get_uint64(argv[3], &tx_value) != 0) {
        shell_fprintf(shell, SHELL_WARNING, "invalid argument: amount\n");
        return -1;
    }
    // tokens to transfer
    uint256_t decimals_u256;
    clear256(&decimals_u256);
    set256_uint64(&decimals_u256, ipow(10, tkn_p->decimals));
    uint256_t tx_value_u256;
    clear256(&tx_value_u256);
    set256_uint64(&tx_value_u256, tx_value);
    uint256_t tx_result_u256;
    // mul256 requires LE values
    mul256(&tx_value_u256, &decimals_u256, &tx_result_u256);
    // convert to BE
    writeu256BE(&tx_result_u256, (uint8_t*)&tx_value_u256);
    // build the transaction
    assert(tkn_p != NULL);
    tx.nonce = acc->nonce;
    tx.gas_price = acc->gas_price;
    tx.gas_limit = 35427;
    tx.to = tkn_p->address;
    clear256(&tx.value);

    data_block_t tx_data;
    uint8_t datablk[256+4];
    tx_data.data = datablk;
    tx_data.data_len = sizeof(datablk);
    int encoded_data_len = Token_transfer(&address_to, &tx_value_u256, &tx_data);
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
        shell_fprintf(shell, SHELL_WARNING, "Error: eth_sendRawTransaction()\n");
        return -1;
    }
    printk_uint256(shell, &tx_hash);

    return 0;
}

static int _token_decimals(const address_t *addr, uint8_t *out)
{
    transaction_t tx;
    memset(&tx, 0, sizeof(tx));
    account_t *acc = wallet_get_account();
    assert(acc != NULL);

    data_block_t tx_data;
    uint8_t datablk[256+4];
    tx_data.data = datablk;
    tx_data.data_len = sizeof(datablk);
    int encoded_data_len = Token_decimals(&tx_data);
    if(encoded_data_len < 0) {
        return -1;
    }
    tx.data = datablk;
    tx.data_len = encoded_data_len;
    tx.to = *addr;

    uint256_t out_u256;
    if(web3_eth_call(&acc->address, &tx, &out_u256, TX_NO_FROM | TX_NO_GAS | TX_NO_GASPRICE | TX_NO_VALUE) < 0) {
        return -1;
    }

    *out = LOWER(LOWER(out_u256));
    return 0;
}

static int erc20_totalSupply(const struct shell *shell, size_t argc, char *argv[])
{
    transaction_t tx;
    memset(&tx, 0, sizeof(tx));
    account_t *acc = wallet_get_account();
    assert(acc != NULL);
    // first parameter is recipient address
    if(argc != 2) {
        printk("missing argument: token index\n");
        return -1;
    }
    token_nfo_t *tkn_p = NULL;
    if(_get_token_by_index(shell, argv[1], &tkn_p) != 0) {
        return -1;
    }
    // build the transaction

    data_block_t tx_data;
    uint8_t datablk[256+4];
    tx_data.data = datablk;
    tx_data.data_len = sizeof(datablk);
    int encoded_data_len = Token_totalSupply(&tx_data);
    if(encoded_data_len < 0) {
        shell_fprintf(shell, SHELL_WARNING, "Can't encode Token::totalSupply()\n");
        return -1;
    }
    tx.data = datablk;
    tx.data_len = encoded_data_len;
    tx.to = tkn_p->address;

    uint256_t out;
    if(web3_eth_call(&acc->address, &tx, &out, TX_NO_FROM | TX_NO_GAS | TX_NO_GASPRICE | TX_NO_VALUE) < 0) {
        return -1;
    }

    shell_print_decimal_u256(shell, &out, tkn_p->decimals);

    return 0;
}

static int erc20_add(const struct shell *shell, size_t argc, char *argv[])
{
    address_t addr;
    // first parameter is recipient address
    if(argc < 2) {
        printk("missing argument: address\n");
        return -1;
    }
    if(hextobin(argv[1], (uint8_t*)&addr, sizeof(addr)) < 0) {
        printk("invalid argument: address\n");
        return -1;
    }
    uint8_t decimals = 0;
    if(_token_decimals(&addr, &decimals) < 0) {
        shell_fprintf(shell, SHELL_WARNING, "couldn't get token decimals\n");
        return -1;
    }
    int ret = _token_add(&addr, decimals, "TKN");
    if(ret < 0) {
        shell_fprintf(shell, SHELL_WARNING, "can't add token\n");
        return -1;
    }
    return 0;
}

static int erc20_list(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    // first parameter is recipient address
    for(int i = 0; i < TOKENS_MAX; i++) {
        token_nfo_t *nfo = &_tokens[i];
        if(nfo->used == 0) { continue; }
        shell_fprintf(shell, SHELL_NORMAL, "%d\t%s\t%u\t", i, nfo->symbol, nfo->decimals);
        printk_hex(shell, (uint8_t*)&nfo->address, sizeof(address_t));
        shell_fprintf(shell, SHELL_NORMAL, "\n");
    }
    return 0;
}

SHELL_CREATE_STATIC_SUBCMD_SET(sub_erc20) {
    SHELL_CMD(add, NULL, "add", erc20_add),
    SHELL_CMD(balance, NULL, "balance", erc20_balance),
    SHELL_CMD(list, NULL, "list", erc20_list),
    SHELL_CMD(transfer, NULL, "transfer", erc20_transfer),
    SHELL_CMD(totalSupply, NULL, "total supply", erc20_totalSupply),
	SHELL_SUBCMD_SET_END
};
SHELL_CMD_REGISTER(erc20, &sub_erc20, "erc20 utils", NULL);

void erc20_shell_register()
{}
