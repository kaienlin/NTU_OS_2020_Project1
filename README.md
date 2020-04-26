# NTU_OS_2020_Project1

a user-space scheduler based on the priority-driven scheduler built in Linux kernel for a set of child processes

---

## Environment

- **Kernel Version**: 4.14.25
- **Platform**: Ubuntu 16.04

## 執行方式
- 將kernel_files/裏面的檔案放到正確的地方，並重新編譯kernel
- 執行```make```
- ```sudo ./scheduler < OS_PJ1_Test/${test_name}```

## 設計

### Overall Structure
- 使用2顆 CPU，一顆(0)用來跑 Scheduler，另一顆(1)用來跑 children processes。
- scheduler.c 只負責 read and preprocess inputs, 將執行的 CPU 及 priority 調整好，就按照policy call對應的函數。對於每一種 Policy，其對應的 scheduler function 定義在獨立的檔案，例如 RR_scheduler 就定義在 RR.c 裏面。
- proprocessing 的部分包含將 processes 按照 ready time 升序排序，這樣能更方便後續處理 processes 的 arrival。如果出現 ready time 相等的情況，則按照在 input 裏的出現順序決定。

### Process Control

我寫了一些 functions 來協助我進行 processes 的控制：

+ ```proc_set_priority```: 利用 ```sched_setscheduler``` 設定指定的 priority。
+ ```proc_set_cpu```: 利用 ```sched_setaffinity``` 讓指定的 process 跑在指定的 process 上
+ ```proc_start```: fork 出新的process出來，爲了避免他在 scheduler 還沒處理前先跑，一 fork 出來就立刻把它的 priority 設成最低。
+ ```proc_block```: 把一個 process 的 priority 調成最低。
+ ```proc_wakeup```: 把一個 process 的 priority 調成最高。
+ ```proc_term```: 把一個已經結束的 process wait 掉。

### FIFO_scheduler

+ 因爲在上面提到的預處理階段，已經把 processes 按照他們的 ready time 排序過了。所以我們可以直接依照他們在 array 裏的順序來處理。
+ 維護一個整數 ready_p，負責檢查在當下的時間點有哪些 process 已經 ready。
+ 維護一個整數 cur_p，負責處理「正在 run 的 process」。如果 cur_p 指到的 process 是 ready 的狀態，就把它跑起來。如果是在 running 的狀態，就把 remaining_time 減一。如果 remaining_time 爲0,就將其終止(wait)，並把 CPU assign 給下一個 process。

### RR_scheduler

### SJF_scheduler

### PSJF_scheduler

