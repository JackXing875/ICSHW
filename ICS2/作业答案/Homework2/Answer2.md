# Homework2 参考答案

## Problem 1

```c
void foo(int n) {
     int i;
     for (i = 0; i < n; i++)
            Fork();
     printf("hello\n");
     exit(0);
}
```

循环里每个进程每轮都 fork 一次，进程数每轮翻倍：
开始 1 个进程，n 轮之后变成 $2^n$ 个进程。
每个进程最后都执行一次 printf，所以共打印 $2^n$ 行。

答案：$2^n$ 行。

## Problem 2

```c
int main() {
      if (fork() == 0) {
             printf("a"); fflush(stdout);
             exit(0);
      } else {
             printf("b"); fflush(stdout);
             waitpid(-1, NULL, 0);
      }
      printf("c"); fflush(stdout);
      exit(0);
}
```

子进程打印 a 后 exit，不会执行到 c。
父进程打印 b，然后 waitpid 阻塞等子进程结束，再打印 c。

a 和 b 由两个并发进程打印，先后不定；c 一定在父进程 waitpid 返回后，
即一定在 a、b 都打印之后。

可能的输出序列：abc、bac。

## Problem 3

```c
int x = 3;
if (Fork() != 0)
      printf("x=%d\n", ++x);
printf("x=%d\n", --x); exit(0);
```

父进程：Fork() != 0 成立，++x 使 x=4，打印 x=4；再 --x 使 x=3，打印 x=3。
子进程：Fork()==0，跳过 if；再 --x 使 x=2，打印 x=2。

父进程两行的相对顺序固定（先 x=4 再 x=3），子进程的 x=2 可以插在任意位置。
可能的输出（按行）：

```text
x=4      x=4      x=2
x=3  或  x=2  或  x=4
x=2      x=3      x=3
```

即三种：(x=4, x=3, x=2)、(x=4, x=2, x=3)、(x=2, x=4, x=3)。

## Problem 4

```c
void end(void) { printf("2"); fflush(stdout); }
int main() {
      if (Fork() == 0)
             atexit(end);
      if (Fork() == 0) { printf("0"); fflush(stdout); }
      else            { printf("1"); fflush(stdout); }
      exit(0);
}
```

进程分析（第一次 fork 记父 P、子 C1；C1 在第一次 fork 后注册了 end）：
第二次 fork 后共 4 个进程，end 处理函数被注册情况随 fork 继承：

```text
进程         注册了 end?   第二个 if 分支    退出时输出
P            否            else -> "1"       1
P 的子 C2    否            if   -> "0"       0
C1           是            else -> "1"       1, 然后 end 打印 2  => 1 2
C1 的子 C3   是            if   -> "0"       0, 然后 end 打印 2  => 0 2
```

四个进程各自的输出序列（进程内有序）：
P: [1]，C2: [0]，C1: [1, 2]，C3: [0, 2]。
总字符为两个 0、两个 1、两个 2。任何合法输出都必须是这四个序列的交错，
且每个 2 前面必须先出现本进程更早的字符（C1 的 2 前要有它的 1，
C3 的 2 前要有它的 0）。所以任何输出不能以 2 开头。

逐项判断：

```text
A. 112002  可能：1(C1) 1(P) 2(C1) 0(C3) 0(C2) 2(C3)        合法
B. 211020  不可能：以 2 开头，没有进程能先输出 2
C. 102120  可能：1(C1) 0(C3) 2(C1) 1(P) 2(C3) 0(C2)        合法
D. 122001  不可能：第 2、3 个字符都是 2 且前面没有 0，
                    C3 的 2 缺少在前的 0
E. 100212  可能：1(P) 0(C3) 0(C2) 2(C3) 1(C1) 2(C1)        合法
```

答案：可能的是 A、C、E；不可能的是 B、D。
