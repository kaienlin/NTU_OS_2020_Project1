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

int get_min_remaining_time_p(Process proc_list[], int N)
{
        unsigned long mn = ULONG_MAX;
        int ret = -1;
        for (int i = 0; i < N; i++) {
                if (proc_list[i].state != READY) continue;
                if (proc_list[i].remaining_time < mn)
                        mn = proc_list[i].remaining_time, ret = i;
        }
        return ret;
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
                sched_yield();
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
                // block the new process immediately
                proc_set_cpu(pid, CPU_CHILDREN);
                proc_block(proc);

                // print to stdout
                printf("%s %d\n", proc->name, pid);
                fflush(stdout);

                // set process attributes
                proc->pid = pid;
                proc->state = READY;
        } else {
                ERR_EXIT("fork");
        }
}

void proc_set_priority(pid_t pid, int priority)
{
        struct sched_param param;
        param.sched_priority = priority;
        if (sched_setscheduler(pid, SCHED_FIFO, &param) != 0)
                ERR_EXIT("sched_setscheduler");
}

void proc_set_other(pid_t pid)
{
        struct sched_param param;
        param.sched_priority = 0;
        if (sched_setscheduler(pid, SCHED_OTHER, &param) != 0)
                ERR_EXIT("sched_setscheduler");
}

void proc_block(Process *proc)
{
        proc_set_other(proc->pid);
        proc->state = READY;
}

void proc_wakeup(Process *proc)
{
        proc_set_priority(proc->pid, PRIORITY_HIGH);
        proc->state = RUNNING;
}

void proc_term(Process *proc)
{
        waitpid(proc->pid, NULL, 0);
        proc->state = TERMINATED;
}

pid_t proc_dummy()
{
        pid_t pid = fork();
        if (pid == 0) {
                proc_set_cpu(getpid(), CPU_CHILDREN);
                proc_set_other(getpid());
                nice(-10);
                while (1);
                return 0;
        } else if (pid > 0) {
                proc_set_cpu(pid, CPU_CHILDREN);
                proc_set_other(pid);
                return pid;
        } else {
                ERR_EXIT("fork");
                return -1;
        }
}
