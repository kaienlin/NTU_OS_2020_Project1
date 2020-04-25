#ifndef _OSPJ1_SCHED_H_
#define _OSPJ1_SCHED_H_

#define _GNU_SOURCE

#include "utils.h"

void FIFO_scheduler(Process proc_list[], int N);
void RR_scheduler(Process proc_list[], int N);
void SJF_scheduler(Process proc_list[], int N);
void PSJF_scheduler(Process proc_list[], int N);

#endif