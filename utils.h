#ifndef _OSPJ1_UTILS_H_
#define _OSPJ1_UTILS_H_

#define _GNU_SOURCE

#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/types.h>

#define TIME_UNIT { volatile unsigned long i; for(i=0;i<1000000UL;i++); }

#define ERR_EXIT(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define CPU_ID 0

#define PRIORITY_HIGH 99

#define SYSCALL_GETTIME 333
#define SYSCALL_SHOWINFO 334

enum Proc_state { READY, RUNNING, TERMINATED };

typedef struct _Process {
        char name[32];                // process name
        unsigned long ready_time;     // fork at this time
        unsigned long exec_time;      // job length
        unsigned long remaining_time; // remaining time
        int ord;                      // the order in the input
        pid_t pid;                    // process id
        enum Proc_state state;        // current state of process
} Process;

int cmp_readytime_asc(const void *_a, const void *_b);
void proc_set_cpu(pid_t pid, int cpu_id);
void proc_start(Process *proc);
void proc_block(Process *proc);
void proc_wakeup(Process *proc);
void proc_term(Process *proc);

#endif // _OSPJ1_UTILS_H_