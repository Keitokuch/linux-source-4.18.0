#include "sched.h"
#include <linux/syscalls.h>

#include <linux/sched.h>
#include <linux/kmod.h>
#include <uapi/linux/sched/types.h>


int jc_is_logging = 0;
EXPORT_SYMBOL(jc_is_logging);

static int usermode_init(struct subprocess_info *info, struct cred *new)
{
    struct sched_param param = { 80 };
    int err;

    if (err = sched_setscheduler_nocheck(current, SCHED_FIFO, &param)){
        return err;
    }

    printk("setsched succeeded");
    return 0;
}

SYSCALL_DEFINE1(jc_sched, int, start)
{
    if (start) {
        /* jc_is_logging = 1; */
        printk("Sched logger started.");
    }
    else {
        /* jc_is_logging = 0; */
        printk("Sched logger stopped.");
    }

    struct subprocess_info *info;
    int ret;
    char *argv[] = { "/root/ret205", "help!", NULL };
    static char *envp[] = {
        "HOME=/",
        "TERM=linux",
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", 
        NULL
    };

    info = call_usermodehelper_setup(argv[0], argv, envp, GFP_KERNEL, usermode_init, NULL, NULL);
    if (!info) {
        printk("error setup");
        return -1;
    }

    ret = call_usermodehelper_exec(info, UMH_WAIT_PROC);
    if (512 == ret)
        printk("Success");
    else
        printk("Failure");

    return 0;

    /* if (512 == call_usermodehelper( argv[0], argv, envp, UMH_WAIT_PROC )) { */
        /* printk("Success"); */
    /* } else { */
        /* printk("Failure"); */
    /* } */
    /* return 0; */
}
