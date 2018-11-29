/**
* @brief 
* @file demo_shell.c
* @author J.H. 
* @date 2018-11-29
*/

/* system includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <zephyr.h>
#include <kernel.h>
#include <shell/shell.h>
#pragma GCC diagnostic pop
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* local includes */
//
// thread stack
#define UPLOAD_STACK_SIZE   512
K_THREAD_STACK_DEFINE(upload_stack, UPLOAD_STACK_SIZE);
static struct k_thread upload_thread_data;

// interval (seconds)
#define DEFAULT_UPLOAD_INTERVAL 30
static uint16_t _upload_interval = DEFAULT_UPLOAD_INTERVAL;

// thread semaphore. it is taken when the thread is spawned and released on exit
K_SEM_DEFINE(upload_thread_sem, 1, 1);
K_SEM_DEFINE(upload_thread_exit_sem, 0, 1);

static void upload_main()
{
    int ret __attribute__((unused)) = k_sem_take(&upload_thread_sem, K_MSEC(1));
    assert(ret == 0);
    char cmdbuf[64];
    // register an account
    snprintf(cmdbuf, sizeof(cmdbuf), "st create_agent A-%d", sys_rand32_get() % 10000);
    shell_execute_cmd(NULL, cmdbuf);
    // generate a random record
    uint32_t rnd = sys_rand32_get();
    snprintf(cmdbuf, sizeof(cmdbuf), "st record id R-%d", rnd % 10000);
    shell_execute_cmd(NULL, cmdbuf);
    snprintf(cmdbuf, sizeof(cmdbuf), "st record species FA%c", 'A' + rnd % 27);
    shell_execute_cmd(NULL, cmdbuf);
    // register a new record
    shell_execute_cmd(NULL, "st create_record");
    // periodically update the record N times or until stop signal is caught
    while(true) {
        shell_execute_cmd(NULL, "st update_record");
        if(k_sem_take(&upload_thread_exit_sem, _upload_interval * 1000) == 0) { break; }
    }
    k_sem_give(&upload_thread_exit_sem);
    k_sem_give(&upload_thread_sem);
}


static int upload_stop(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    ARG_UNUSED(shell);
    if(k_sem_take(&upload_thread_sem, K_MSEC(5)) == 0) {
        printk("thread not running\n");
        k_sem_give(&upload_thread_sem);
        return 0;
    }
    // signal thread to exit
    k_sem_give(&upload_thread_exit_sem);
    // wait for the exit
    k_sem_take(&upload_thread_sem, K_FOREVER);
    // reset the semaphores
    k_sem_give(&upload_thread_sem);
    k_sem_take(&upload_thread_exit_sem, K_FOREVER);
    return 0;
}

static int upload_start(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(shell);
    if(argc == 2) {
        _upload_interval = strtoul(argv[1], NULL, 10);
        if(errno != 0) {
            printk("invalid upload interval\n");
            return -1;
        }
    } else {
        _upload_interval = DEFAULT_UPLOAD_INTERVAL;
    }
    if(k_sem_take(&upload_thread_sem, K_MSEC(5)) != 0) {
        printk("thread already started\n");
        return 0;
    }
    k_thread_create(
            &upload_thread_data,
            upload_stack, K_THREAD_STACK_SIZEOF(upload_stack),
            (k_thread_entry_t)upload_main, NULL, NULL, NULL,
            5, 0, K_NO_WAIT);
    return 0;
}

SHELL_CREATE_STATIC_SUBCMD_SET(sub_upload) {
    SHELL_CMD(start, NULL, "start supply chain demo", upload_start),
    SHELL_CMD(stop, NULL, "stop supply chain demo", upload_stop),
	SHELL_SUBCMD_SET_END
};
SHELL_CMD_REGISTER(sc_demo, &sub_upload, "sawtooth supply chain demo", NULL);
