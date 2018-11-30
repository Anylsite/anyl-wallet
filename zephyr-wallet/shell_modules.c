/**
* @brief 
* @file shell_modules.c
* @author J.H. 
* @date 2018-11-29
*/

/* system includes */

/* local includes */
#include "shell_modules.h"
#include "web3_shell.h"
#include "sensor_service.h"
#include "upload.h"
#include "erc20_shell.h"
#include "sntp_shell.h"

void wallet_register_shell_modules()
{
    web3_shell_register();
    sensor_shell_register();
    upload_shell_register();
    erc20_shell_register();
    sntp_shell_register();
}
