# Homework1

## Problem 1

Specify which kind of exception will occur in the given scenario, and point out whether it’s asynchronous or synchronous. 

a)  A page fault 
- 故障 (fault)，是同步 (synchronous) 的

b)  You receive an E-mail from the Internet 
- 中断 (interrupt)， 是异步 (asynchronous) 的

c)  You’re running a command “kill -9 <pid>” in your shell 
- 陷阱 (trap), 是同步 (synchronous) 的

d) The memory of your pc corrupted 
- 终止 (abort)，是同步 (synchronous) 的

## Problem 2

Write the assembly code of the code segment below. (Supposing we are directly using syscall to call KILL, which has syscall number 37) 
```c
int main() { 
    kill(2024, 3); 
    exit(0); 
} 
```
The answer is:

```bash
.section .text
.global main
main:
    mov rax, 37
    mov rdi, 2024
    mov rsi, 3
    syscall

    mov rax, 60
    mov rdi, 0
    syscall
```