#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage void sys_ospj1_showinfo(pid_t pid, long st_sec, long st_nsec, long ft_sec, long ft_nsec) {
        printk(KERN_INFO "[Project1] %d %ld.%.9ld %ld.%.9ld\n", pid, st_sec, st_nsec, ft_sec, ft_nsec);
}
