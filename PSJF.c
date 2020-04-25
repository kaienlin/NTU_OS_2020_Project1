#include "utils.h"
#include "data_struct.h"

void PSJF_scheduler(Process proc_list[], int N)
{
        int left_jobs = N;
        int ready_p = 0;
        int running_p = -1;

        for (int time = 0; ; time++) {
                // start all process whose ready time <= time
                while (ready_p < N && proc_list[ready_p].ready_time <= time) {
                        proc_start(&proc_list[ready_p]);
                        ++ready_p;
                }
                
                int mn_p = get_min_remaining_time_p(proc_list, N);
                // if CPU is available, assign a process to it
                if (running_p == -1) {
                        running_p = mn_p;
                        if (running_p != -1) {
                                proc_wakeup(&proc_list[running_p]);
                        }
                } else if (proc_list[mn_p].remaining_time < proc_list[running_p].remaining_time) {
                        proc_block(&proc_list[running_p]);
                        running_p = mn_p;
                        proc_wakeup(&proc_list[running_p]);
                }

                TIME_UNIT;

                if (running_p != -1) {
                        --proc_list[running_p].remaining_time;
                        if (proc_list[running_p].remaining_time == 0) {
                                proc_term(&proc_list[running_p]);
                                running_p = -1;
                                --left_jobs;
                                if (left_jobs == 0)
                                        exit(EXIT_SUCCESS);
                        }
                }
        }
}