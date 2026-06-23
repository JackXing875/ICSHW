# Homework5 参考答案

## Problem 1

这是 CS:APP 9.6.4 的示例存储系统：虚拟地址 14 位，物理地址 12 位，页大小 64B
（页内偏移 6 位）；TLB 四路组相联、共 16 项（4 组，TLBI 2 位、TLBT 6 位）；
cache 直接映射、16 行、每行 4 字节（CO 2 位、CI 4 位、CT 6 位）。

给定 virtual address 0x03FC。

A. Virtual address format（14 位）

```text
位:   13 12 11 10  9  8  7  6 | 5  4  3  2  1  0
值:    0  0  0  0  1  1  1  1 | 1  1  1  1  0  0
      \-------- VPN --------/  \---- VPO ------/
0x03FC = 00 0011 1111 1100
```

B. Address translation

```text
参数            值
VPN            0x0F          (bits 13-6 = 0000 1111)
TLB index      0x3           (bits 7-6 = 11)
TLB tag        0x03          (bits 13-8 = 000011)
TLB hit?(Y/N)  Y             (第 3 组中有 tag=03、PPN=0D、valid=1 的项)
Page fault?(Y/N) N
PPN            0x0D
```

C. Physical address format（12 位）

```text
位:   11 10  9  8  7  6 | 5  4  3  2  1  0
值:    0  0  1  1  0  1 | 1  1  1  1  0  0
      \---- PPN ----/    \---- PPO ----/
PA = PPN:PPO = 0x0D : 0x3C = 0x37C
```

D. Physical memory reference

```text
参数                值
Byte offset (CO)    0x0       (PA bits 1-0 = 00)
Cache index (CI)    0xF       (PA bits 5-2 = 1111)
Cache tag (CT)      0x0D      (PA bits 11-6 = 001101)
Cache hit?(Y/N)     N         (第 0xF 行存的 tag=0x14 ≠ 0x0D)
Cache byte returned 无        (cache miss，不返回字节)
```

## Problem 2

64 位虚拟地址空间，page size 2048B = $2^{11}$，PTE = 4B，每张页表占一个 page。

页内偏移 = $\log_2 2048 = 11$ 位。
每张页表是一个 page，可放 $2048 / 4 = 512 = 2^9$ 个 PTE，所以每级索引 9 位。
需要索引的虚页号位数 = $64 - 11 = 53$ 位。
级数 = $\lceil 53 / 9 \rceil = 6$。

验证：6 级共能索引 $6 \times 9 = 54 \ge 53$ 位；最高一级只用到 $53 - 5 \times 9 = 8$ 位
（即顶级页表只用 256 项，但仍占满一个 page 的空间）。

答案：需要 6 级页表。

## Problem 3

VA 空间 1MB（20 位），PA 空间 8MB（23 位）。用 VA 最高两位选段：
00=code，01=data，10=heap，11=stack；其余 18 位为段内偏移（每段 VA 空间
$2^{18}$ = 256KB）。寄存器：

```text
segment  base   size    grows
code     4MB    4KB     正向(1)
data     2MB    2KB     正向(1)
heap     7MB    8KB     正向(1)
stack    1MB    256KB   反向(0)
```

程序中 `char s[128*1024]` 是 main 里的局部数组，分配在栈上。

### 1) 数据结构在哪一段

s 是局部数组，存放在 stack 段。

### 2) 求各 VA 的 PA

s 的 VA = 0xC1300：

```text
0xC1300 = 1100 0001 0011 0000 0000  (20 位)
最高两位 = 11 -> stack 段
段内偏移 = 低 18 位 = 0x01300 = 4864
```

stack 反向增长，PA = base - (段最大长度 - 偏移)，段最大长度 = $2^{18}$ = 256KB：

```text
PA = 1MB - (256KB - 4864)
   = 1048576 - (262144 - 4864)
   = 1048576 - 257280
   = 791296 = 0xC1300
```

stack 物理范围 [1MB-256KB, 1MB) = [768KB, 1MB)，791296 在其中，合法。
所以 s 对应 PA = 0xC1300。

VA = 258K：

```text
258K = 264192 = 0x40800 = 0100 0000 1000 0000 0000
最高两位 = 01 -> data 段
段内偏移 = 0x00800 = 2048
```

data 正向增长，合法偏移范围 [0, size) = [0, 2048)。偏移 2048 恰好等于 size，
越界，触发段错误，没有有效 PA。

VA = 514K：

```text
514K = 526336 = 0x80800 = 1000 0000 1000 0000 0000
最高两位 = 10 -> heap 段
段内偏移 = 0x00800 = 2048
```

heap 正向增长，2048 < 8192，合法。

```text
PA = base + 偏移 = 7MB + 2048 = 0x700000 + 0x800 = 0x700800
```

## Problem 4

系统参数：16 位虚拟地址；物理内存 16KB（物理地址 14 位）；页大小 64B
（页内偏移 6 位）；两级页表，每张表 64B = 32 项（每级索引 5 位），表项 2B。
虚拟地址划分：

```text
[ VPN1: bit15-11 (5) ][ VPN2: bit10-6 (5) ][ offset: bit5-0 (6) ]
```

PDE 低位为 P，高位是页表基址（页对齐，清掉低 6 位即得基址）。
PTE 低 3 位为 U、W、P（bit2、bit1、bit0），高位是物理页地址（清低 6 位）。
权限：P=present；W=可写（内核态也受其约束）；U=用户态可访问。

相关内存内容（十六进制）：

```text
0130: 2101   019C: 1201   210C: 3A47   1214: 27C1
```

### 1) Process 1（用户态，page directory base 0x0100）写 0xC1B2

```text
0xC1B2 = 1100 0001 1011 0010
VPN1 = 11000 = 0x18 = 24
VPN2 = 00110 = 6
offset = 110010 = 0x32

(a) PDE 地址 = 0x0100 + 24*2 = 0x0130
    mem[0x0130] = 0x2101 -> P=1，页表基址 = 0x2100
(b) PTE 地址 = 0x2100 + 6*2  = 0x210C
    mem[0x210C] = 0x3A47 -> 标志 0x47 = ...0100 0111 -> P=1, W=1, U=1
    物理页地址 = 0x3A40
(c) 用户态写、U=1、W=1、P=1 -> 允许；PA = 0x3A40 | 0x32 = 0x3A72
(d) success
```

### 2) Process 2（内核态，page directory base 0x0180）写 0x728F

```text
0x728F = 0111 0010 1000 1111
VPN1 = 01110 = 0x0E = 14
VPN2 = 01010 = 0x0A = 10
offset = 001111 = 0x0F

(a) PDE 地址 = 0x0180 + 14*2 = 0x019C
    mem[0x019C] = 0x1201 -> P=1，页表基址 = 0x1200
(b) PTE 地址 = 0x1200 + 10*2 = 0x1214
    mem[0x1214] = 0x27C1 -> 标志 0xC1 = 1100 0001 -> P=1, W=0, U=0
(c) 该页 P=1 但 W=0，写操作不被允许（W 对内核态同样生效），
    翻译不产生可用于本次写的有效地址：NONE
    出错表项地址：PTE 0x1214
(d) page not writable
```
