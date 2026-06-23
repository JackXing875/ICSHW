# Homework1 参考答案

## Problem 1

判断每种场景属于哪类 exception，以及是 asynchronous 还是 synchronous。
按 CS:APP 的分类：Interrupt 是 asynchronous，Trap / Fault / Abort 都是
synchronous（由执行指令引发）。

a) A page fault
访问的页不在内存中，由当前指令的访存触发，属于 fault，synchronous。
处理后通常重新执行触发的那条指令。

b) You receive an E-mail from the Internet
网卡这个 I/O 设备产生中断，属于 interrupt，asynchronous。
与当前执行的指令无关。

c) Running "kill -9 <pid>" in your shell
shell 进程执行 kill 这个 system call，陷入内核，属于 trap，synchronous。
（被杀进程随后会收到内核投递的 SIGKILL 而终止，那是信号机制，
不是“运行该命令”本身直接产生的 exception。）

d) The memory of your pc corrupted
硬件层面不可恢复的存储错误（如奇偶校验 / machine check），属于 abort。
按 CS:APP 的分类表 abort 归为 synchronous，处理后不再返回。

## Problem 2

把下面代码写成汇编（直接用 syscall 调用 KILL，syscall 号为 37）。

```c
int main() {
      kill(2024, 3);
      exit(0);
}
```

x86-64 / Linux 调用约定：syscall 号放 rax，参数依次放 rdi、rsi、rdx…，
用 syscall 指令陷入内核。kill 号题目给定为 37；exit 号为 60。

```asm
main:
        # kill(2024, 3)
        movq    $37,   %rax     # syscall number: kill
        movq    $2024, %rdi     # arg1: pid
        movq    $3,    %rsi     # arg2: sig
        syscall

        # exit(0)
        movq    $60,   %rax     # syscall number: exit
        movq    $0,    %rdi     # arg1: status
        syscall
```

说明：题目只给出了 KILL 的 syscall 号 37，exit 的号在 x86-64 Linux 下是 60。
若课程约定的是 32 位约定（int $0x80，参数放 ebx/ecx/...），形式类似，
只是把 rax/rdi/rsi 换成 eax/ebx/ecx，并用 int $0x80 代替 syscall。
