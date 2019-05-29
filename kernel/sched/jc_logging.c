#include "sched.h"
#include <linux/syscalls.h>

int jc_is_logging = 0;
EXPORT_SYMBOL(jc_is_logging);

SYSCALL_DEFINE1(jc_sched, int, start)
{
    if (start) {
        jc_is_logging = 1;
        printk("Sched logger started.");
    }
    else {
        jc_is_logging = 0;
        printk("Sched logger stopped.");
    }

    return 0;
}
