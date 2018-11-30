/**
* @brief 
* @file sntp_shell.c
* @author J.H. 
* @date 2018-11-29
*/

/* system includes */
#define LOG_MODULE_NAME sntp_shell
#include <net/sntp.h>
#include <shell/shell.h>
#include <posix/time.h>

/* local includes */

#define SNTP_PORT 123


#include <misc/printk.h>
void resp_callback(struct sntp_ctx *ctx,
		   int status,
		   u64_t epoch_time,
		   void *user_data)
{
    struct timespec ts = {
        .tv_sec = epoch_time,
        .tv_nsec = 0
    };
    clock_settime(CLOCK_REALTIME, &ts);
    struct k_sem *sync_sem = (struct k_sem *)user_data;
    k_sem_give(sync_sem);
}

static int shell_sntp_sync(const struct shell *shell, size_t argc, char *argv[])
{
	struct sntp_ctx ctx;
	int rv;
    // semaphore to keep the app alive until the reply is received or timeouts
    struct k_sem sync_sem;
    k_sem_init(&sync_sem, 0, 1);

	/* ipv4 */
	rv = sntp_init(&ctx,
		       CONFIG_NET_CONFIG_PEER_IPV4_ADDR,
		       SNTP_PORT,
		       K_FOREVER);
	if (rv < 0) {
		shell_error(shell, "Failed to init sntp ctx: %d\n", rv);
		goto end;
	}
	rv = sntp_request(&ctx, K_FOREVER, resp_callback, &sync_sem);
	if (rv < 0) {
		shell_warn(shell, "Failed to send sntp request: %d\n", rv);
		goto end;
	}
    k_sem_take(&sync_sem, K_FOREVER);
end:
	sntp_close(&ctx);
    return 0;
}

static int shell_sntp_time(const struct shell *shell, size_t argc, char *argv[])
{
    ARG_UNUSED(argv);
    ARG_UNUSED(argc);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    shell_print(shell, "%ld", tv.tv_sec);
    return 0;
}

SHELL_CREATE_STATIC_SUBCMD_SET(sub_sntp) {
    SHELL_CMD(sync, NULL, "sync", shell_sntp_sync),
    SHELL_CMD(time, NULL, "time", shell_sntp_time),
	SHELL_SUBCMD_SET_END
};
SHELL_CMD_REGISTER(sntp, &sub_sntp, "sntp utils", NULL);

void sntp_shell_register()
{}
