#include "utils.h"

void FIFO_scheduler(Process proc_list[], int N)
{
        int left_jobs = N;
        int cur_p = 0;
        int ready_p = 0;
        for (int time = 0; left_jobs > 0; time++) {
                // assertion to ensure correctness
                assert(cur_p <= ready_p);

                // start all process whose ready time <= time
                while (ready_p < N && proc_list[ready_p].ready_time <= time) {
                        proc_start(&proc_list[ready_p]);
                        ++ready_p;
                }

                // if the current process is ready, wake up it
                if (proc_list[cur_p].state == READY) {
                        proc_wakeup(&proc_list[cur_p]);
                }

                TIME_UNIT;
                
                // if the current process is running, decrease its remaining time
                if (proc_list[cur_p].state == RUNNING) {
                        --proc_list[cur_p].remaining_time;
                        if (proc_list[cur_p].remaining_time == 0) {
                                proc_term(&proc_list[cur_p]);
                                ++cur_p;
                                --left_jobs;
                        }
                }
        }
}