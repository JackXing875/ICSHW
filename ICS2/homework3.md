# Homework3

姓名：邢添珵
学号：2024202862

## Problem 1

下列程序的输出是？
```c
pid_t pid; 
int counter = 2;

void handler1(int sig) { 
    counter = counter - 1; 
    printf("%d", counter); 
    fflush(stdout);
    exit(0); 
} 

int main() {
    signal(SIGUSR1, handler1);
    printf("%d", counter); 	
    fflush(stdout);
    if ((pid = fork()) == 0) { 
        while(1) {}; } 
    kill(pid, SIGUSR1);
    waitpid(-1, NULL, 0); 
    counter = counter + 1; 
    printf("%d", counter); 
    exit(0); 
} 
```

答：输出为 `213` 。开始时打印 `2` ，之后创建子进程，子进程死循环，之后父进程发送信号，子进程接到信号后结束死循环，打印 `1` 并终止，父进程接到子进程终止的信号后打印 `3` 并退出。

## Problem 2

写一个snooze程序，程序从命令行读入一个整数作为参数，程序的功能为睡眠参数秒，然后终止。睡眠期间，用户可以用同时按下ctrl+c终止程序，snooze输出当前睡眠时间。

例如
```bash
unix> ./snooze 5
Slept for 3 of 5secs.
User hits crtl-c after 3 seconds unix> 
```

答：
```c
int snooze(int s) {
    int t = sleep(s);
    printf("User hits ctrl-c after %d seconds", s - t);
}
```

## Problem 3

假定要在一台处理器上执行如下任务：0时刻顺序到来1/2/3任务，需要的执行时间分别是8/3/2；2时刻顺序到来4/5任务，需要的执行时间分别是10/1。给出采用下列调度算法时的调度顺序、平均周转时间和平均响应时间。

1. FCFS
2. RR 时间片为1，每次轮转内部按照到达顺序先到先服务，轮转过程中新来的请求这一次轮转的末尾
3. 抢占式SJF
4. 实际上任务到来时无法知道执行时间，要用MLFQ算法完成调度。涉及到的参数中，每个任务被分配一个时间片就降级，一个周期的长度为10，不同优先级队列的Rr时间片都是1

答：

1) 调度顺序为 `1 2 3 4 5` ，平均周转时间为 $\frac{1}{5} \times (8+11+13+21+22) = 15$， 平均响应时间为 $\frac{1}{5} \times (0+8+11+11+21) = 10.2$

2) 调度顺序为 `1 2 3 4 5 1 2 3 4 1 2 4 1 4 1 4 1 4 1 4 1 4 4 4`， 平均周转时间为 $\frac{1}{5} \times (21 + 11 + 8 + 22 + 3) = 13$， 平均响应时间为 $\frac{1}{5} \times (0+1+2+1+2) = 1.2$

3) 调度顺序为 `3 5 2 1 4` ，平均周转时间为 $\frac{1}{5} \times (14 + 6+2 +22 +1) = 9$，平均响应时间为 $\frac{1}{5} \times (6+3+0+12+0) = 4.2$


4) 调度顺序为 `1 2 3 4 5 1 2 3 4 1 2 4 1 4 1 4 1 4 1 4 1 4 4 4` ，平均周转时间为 $\frac{1}{5} \times (21 + 11 + 8 + 22 + 3) = 13$，平均响应时间为 $\frac{1}{5} \times (0 + 1 + 2 + 1 + 2) = 1.2$

