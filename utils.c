#include "utils.h"

int cmp_readytime_asc(const void *_a, const void *_b)
{
        Process *a = (Process *)_a, *b = (Process *)_b;
        if (a->ready_time < b->ready_time)
                return -1;
        else if (a->ready_time > b->ready_time)
                return 1;
        else if (a->ord < b->ord)
                return -1;
        else if (a->ord > b->ord)
                return 1;
        return 0;
}

void proc_set_cpu(pid_t pid, int cpu_id)
{
        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET(cpu_id, &cpu_set);

        if (sched_setaffinity(pid, sizeof(cpu_set), &cpu_set) != 0)
                ERR_EXIT("sched_setaffinity");
}

void proc_start(Process *proc)
{
        pid_t pid = fork();
        
        if (pid == 0) {  // child process
                proc_set_cpu(getpid(), CPU_CHILDREN);
                // get starting time
                long st_sec, st_nsec;
                syscall(SYSCALL_GETTIME, &st_sec, &st_nsec);

                for (int i = 0; i < proc->exec_time; i++)
                        TIME_UNIT;
                
                // get finishing time
                long ft_sec, ft_nsec;
                syscall(SYSCALL_GETTIME, &ft_sec, &ft_nsec);

                // print info to dmesg
                syscall(SYSCALL_SHOWINFO, getpid(), st_sec, st_nsec, ft_sec, ft_nsec);

                exit(EXIT_SUCCESS);
        } else if (pid > 0) {  // parent process
                // print to stdout
                printf("%s %d\n", proc->name, pid);
                fflush(stdout);

                proc_set_cpu(pid, CPU_CHILDREN);

                // set process attributes
                proc->pid = pid;
                proc->state = READY;
        } else {
                ERR_EXIT("fork");
        }
}

void proc_block(Process *proc)
{
        struct sched_param param;
        param.sched_priority = 0;
        if (sched_setscheduler(proc->pid, SCHED_OTHER, &param) != 0)
                ERR_EXIT("sched_setscheduler");
        proc->state = READY;
}

void proc_wakeup(Process *proc)
{
        struct sched_param param;
        param.sched_priority = PRIORITY_HIGH;
        if (sched_setscheduler(proc->pid, SCHED_FIFO, &param) != 0)
                ERR_EXIT("sched_setscheduler");
        proc->state = RUNNING;
}

void proc_term(Process *proc)
{
        waitpid(proc->pid, NULL, 0);
        proc->state = TERMINATED;
}
