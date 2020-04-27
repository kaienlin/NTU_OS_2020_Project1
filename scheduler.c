#include "scheduler.h"
#include "utils.h"

int main(void)
{
        // declare variables
        char sched_policy[16];
        unsigned long N;
        scanf("%s %lu", sched_policy, &N);
        Process proc_list[N];
        memset(proc_list, 0, sizeof(proc_list));

        // read inputs
        for (int i = 0; i < N; i++) {
                scanf("%s %lu %lu", proc_list[i].name, &proc_list[i].ready_time, &proc_list[i].exec_time);
                proc_list[i].remaining_time = proc_list[i].exec_time;
                proc_list[i].ord = i;
                proc_list[i].state = NOT_ARRIVED;
        }

        // sort processed by their ready time
        qsort(proc_list, N, sizeof(Process), cmp_readytime_asc);

        // run on 0-th CPU
        proc_set_cpu(getpid(), CPU_SCHEDULER);

        // make the scheduler have high priority
        struct sched_param param;
        param.sched_priority = PRIORITY_HIGH;
        if (sched_setscheduler(getpid(), SCHED_FIFO|SCHED_RESET_ON_FORK, &param) != 0)
                ERR_EXIT("sched_setscheduler");

        pid_t dummy = proc_dummy();

        // parse policy and execute the corresponded function
        if (strcmp("FIFO", sched_policy) == 0) {
                FIFO_scheduler(proc_list, N);
        } else if (strcmp("RR", sched_policy) == 0)  {
                RR_scheduler(proc_list, N);
        } else if (strcmp("SJF", sched_policy) == 0) {
                SJF_scheduler(proc_list, N);
        } else if (strcmp("PSJF", sched_policy) == 0) {
                PSJF_scheduler(proc_list, N);
        } else {
                fprintf(stderr, "unknown scheduling policy: %s\n", sched_policy);
                return EXIT_FAILURE;
        }

        kill(dummy, SIGKILL);
        
        return EXIT_SUCCESS;
}