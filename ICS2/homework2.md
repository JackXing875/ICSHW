# Homework2

姓名：邢添珵
学号：2024202862

## Problem 1

下面函数会打印多少行输出？用一个n的函数给出答案（n>=1）
```c
void foo(int n) {
    int i;
    for (i = 0;i<n;i++)
        Fork();
    printf(“hello\n”);
    exit(0);
}
```

答：
会打印 $ 2^n $ 行输出。

## Problem 2

下面的程序可能的输出序列是什么？
```c
int main() {
    if (fork() == 0) {
        printf(“a”);	fflush(stdout);
        exit(0);
    }else {
        printf(“b”);	fflush(stdout);
        waitpid(-1, NULL, 0);
    }
    printf(“c”);	fflush(stdout);
    exit(0);
}
```

答：
可能输出 `b a c` 或 `a b c`。 

## Problem 3

Consider the following program, write down all the possible outputs. 

```c
#include “csapp.h” 
int main() { 
    int x = 3;
    if (fork() != 0) 
        printf("x=%d\n", ++x); 
    printf("x=%d\n", --x); exit(0); 
} 
```

答：可能的输出是 `4 \n 3 \n 2` 或 `2 \n 4 \n 3` 或 `4 \n 2 \n 3` .

## Problem 4

考虑下面的程序：
```c
#include “csapp.h”
void end (void) {
    printf(“2”); fflush(stdtou);
}
Int main() {
    if(Fork() == 0) 
        atexit(end);
    if(Fork() == 0) {
        printf(“0”);
        fflush(stdout);
    }else {
        printf(“1”);
        fflush(stdout);
    }
    exit(0);
}
```

- 判断下面哪个输出是可能的：
A. 112002, B. 211020, C. 102120, D. 122001, E. 100212
        
- Atexit函数以一个只指向函数的指针为输入，并将它添加到函数列表中（初始为空），当exit函数被调用时，会调用该列表中的函数。
        
答：A C E

forf() 出的子进程再 fork() 出的子进程也继承了函数列表，所以也会打印 2.
同时，再打印完两个 2 之前至少会有一次打印 0 和 1 ，所以排除 B D。

附：
思维痕迹：

![思维痕迹](第二次作业.png)