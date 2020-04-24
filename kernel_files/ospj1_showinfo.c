#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/timer.h>

asmlinkage void sys_showinfo(int pid, long st_sec, long st_nsec, long ft_sec, long ft_nsec) {
        printk(KERN_INFO "[Project1] %d %.9ld.%.9ld %.9ld.%.9ld\n", st_sec, st_nsec, ft_sec, ft_nsec);
}