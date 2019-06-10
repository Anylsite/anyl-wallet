/**
* @brief 
* @file main.c
* @author J.H. 
* @date 2018-11-26
*/

/* system includes */
#include <stdint.h>
#include <string.h>
#include <shell/shell.h>

/* local includes */
#include "zephyr-wallet/wallet.h"
#include "zephyr-wallet/shell_modules.h"

extern uint8_t g_zephyr_private_key[32];

void main(void)
{
    privkey_t pk;
    memcpy(&pk.k, g_zephyr_private_key, 32);
    wallet_register_shell_modules();

    wallet_set_global_privkey(&pk);
    shell_execute_cmd(NULL, "help");
}
