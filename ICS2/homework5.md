# Homework5

姓名：邢添珵
学号：2024202862

## Problem 1

根据 9.6.4 的示例内存程序，给定 virtual address `0x03FC`，完成地址翻译。

答：

已知第一页约定 page size 为 `64B`，所以页内偏移 `VPO = 6 bit`。  
虚拟地址一共 14 bit，因此：

- `VPN = 14 - 6 = 8 bit`
- TLB 为 4 组，所以 `TLB index = 2 bit`
- `TLB tag = 8 - 2 = 6 bit`

`0x03FC = 0000 1111 1111 00(2)`，按 14 位写为：

```text
13          6 5          0
00001111      111100
\___VPN___/   \__VPO__/
```

进一步拆分 VPN：

```text
VPN = 00001111
TLB tag = 000011 = 0x03
TLB index = 11 = 0x3
```

查 TLB 第 `3` 组，可见 tag=`03` 的表项有效，命中，得到：

- `PPN = 0x0D`
- 所以不会发生 page fault

因此物理地址为：

$$
PA = (PPN << 6) + VPO = (0x0D << 6) + 0x3C = 0x37C
$$

物理地址 `0x37C` 再做 cache 分解。已知 cache 是 16 组、块大小 4B、直接映射，所以：

- `Byte offset = 2 bit`
- `Cache index = 4 bit`
- `Cache tag = 12 - 4 - 2 = 6 bit`

`0x37C = 0011 0111 1100(2)`，因此：

- `Byte offset = 00 = 0x0`
- `Cache index = 1111 = 0xF`
- `Cache tag = 001101 = 0x0D`

查 cache 的第 `F` 组，可见有效位为 `0`，所以 cache miss。

综上：

| Parameter | Value |
| --- | --- |
| VPN | `0x0F` |
| TLB index | `0x3` |
| TLB tag | `0x03` |
| TLB hit?(Y/N) | `Y` |
| Page fault?(Y/N) | `N` |
| PPN | `0x0D` |
| Byte offset | `0x0` |
| Cache index | `0xF` |
| Cache tag | `0x0D` |
| Cache hit?(Y/N) | `N` |
| Cache byte returned | `N/A` |
| Physical memory reference | `0x37C` |

## Problem 2

计算机有64位虚拟地址空间，page size是2048B，page table entry长度是4B。用multi-level page table把所有页面放在表中，需要多少levels？写出解题步骤。注意页表本身也是以page为单位进行空间分配和管理的，其中第一级页表是一个完整的page。

答：
已知：

- 虚拟地址空间为 64 位，所以虚拟地址总长度是 `64 bit`
- page size = `2048B = 2^11 B`，所以页内偏移占 `11 bit`
- 虚拟页号 `VPN = 64 - 11 = 53 bit`
- page table entry 长度是 `4B = 2^2 B`

由于页表本身也按 page 分配，而一个页表页的大小就是 `2048B`，所以一页页表中能容纳的页表项数为：

$$
\frac{2^{11}}{2^2} = 2^9 = 512
$$

也就是说，每一级页表最多提供 `9 bit` 的索引。

要覆盖全部虚拟页号的 `53 bit`，需要的页表级数 `L` 满足：

$$
9L \ge 53
$$

所以：

$$
L \ge \frac{53}{9} \approx 5.89
$$

向上取整可得：

$$
L = 6
$$

结论：需要 `6` 级页表。

## Problem 3

一个计算机虚拟内存空间大小为 `1MB`，物理内存空间大小为 `8MB`。虚存空间依次分为 4 个 segment（`code`, `data`, `heap`, `stack`），采用 segmentation 内存管理，用 VA 最高两位标记 segment，有寄存器记录下列信息：

| segment | base | size | positive |
| --- | --- | --- | --- |
| code | `4MB` | `4KB` | `1` |
| data | `2MB` | `2KB` | `1` |
| heap | `7MB` | `8KB` | `1` |
| stack | `1MB` | `256KB` | `0` |

请根据给定程序回答问题。

```c
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#define ARRAY_LEN (128*1024)
#define SIZE 100
int a = 0;
int b = 1;

int main(void) {
    int idx;

    char s[ARRAY_LEN];
    printf("s=%#X\n", &s);

    int fd = open("test.txt", O_RDWR);
    read(fd, s, ARRAY_LEN);

    close(fd);
    return 0;
}
```

答：

VA 最高两位标记 segment，因此 1MB 虚拟空间可分为四段，每段大小都是：

$$
\frac{1MB}{4} = 256KB = 2^{18}B
$$

所以 VA 的结构是：

- 最高 2 bit：segment 编号
- 低 18 bit：段内偏移

四段对应的 VA 范围为：

- code：`00xxxxxxxxxxxxxxxxxx`，即 `0x00000 ~ 0x3FFFF`
- data：`01xxxxxxxxxxxxxxxxxx`，即 `0x40000 ~ 0x7FFFF`
- heap：`10xxxxxxxxxxxxxxxxxx`，即 `0x80000 ~ 0xBFFFF`
- stack：`11xxxxxxxxxxxxxxxxxx`，即 `0xC0000 ~ 0xFFFFF`

程序中的数据结构所在段如下：

- `main` 函数、代码本身、字符串常量 `"s=%#X\n"` 和 `"test.txt"` 在 `code` 段
- 全局变量 `a`、`b` 在 `data` 段
- 局部变量 `idx`、`fd` 和局部数组 `s[128*1024]` 在 `stack` 段
- 该程序中没有使用 `heap`

可以画成：

```text
VA space (1MB)

0x00000 ---------------- 0x3FFFF : code
                          - main
                          - 字符串常量

0x40000 ---------------- 0x7FFFF : data
                          - a, b

0x80000 ---------------- 0xBFFFF : heap
                          - 本程序未使用

0xC0000 ---------------- 0xFFFFF : stack
                          - idx, fd, s[128*1024]
```

### 1. 已知程序输出 `s = 0xC1300`，求 `s` 对应的 PA

`0xC1300` 的最高两位是 `11`，所以它属于 `stack` 段。  
段内偏移为：

$$
0xC1300 - 0xC0000 = 0x1300
$$

`stack` 段参数为：

- `base = 1MB = 0x100000`
- `size = 256KB = 0x40000`
- `positive = 0`

`positive = 0` 表示该段向低地址增长，所以：

$$
PA = base - size + offset
$$

代入得：

$$
PA = 0x100000 - 0x40000 + 0x1300 = 0xC1300
$$

所以 `s` 对应的物理地址为：

$$
\boxed{0xC1300}
$$

### 2. 如果 `VA = 258K / 514K`，对应的 PA 是多少

#### `VA = 258K`

$$
258K = 258 \times 1024 = 0x40800
$$

`0x40800` 最高两位是 `01`，属于 `data` 段。  
段内偏移为：

$$
0x40800 - 0x40000 = 0x800
$$

而 `data` 段大小为：

$$
2KB = 0x800
$$

合法偏移范围应为 `0x000 ~ 0x7FF`，`0x800` 已经越界，因此该地址非法，没有对应的物理地址。

结论：`VA = 258K` 时，`PA` 不存在，发生越界访问。

#### `VA = 514K`

$$
514K = 514 \times 1024 = 0x80800
$$

`0x80800` 最高两位是 `10`，属于 `heap` 段。  
段内偏移为：

$$
0x80800 - 0x80000 = 0x800
$$

`heap` 段大小为 `8KB = 0x2000`，所以偏移 `0x800` 合法。  
由于 `heap` 段 `positive = 1`，故：

$$
PA = base + offset = 0x700000 + 0x800 = 0x700800
$$

结论：

- `VA = 258K`：无对应 PA，越界
- `VA = 514K`：`PA = 0x700800`

## Problem 4

题目给定：

- 16-bit 虚拟地址
- 物理内存 `16KB`
- page size 为 `64B`
- 两级页表
- 两级页表每级都是 `64B`，每个表项 `2B`

因此：

- 页内偏移 `VPO = 6 bit`
- 每张页表有 `64 / 2 = 32` 个表项，所以每级索引都是 `5 bit`
- 虚拟地址划分为：`5 bit 一级索引 + 5 bit 二级索引 + 6 bit 页内偏移`

页目录基址：

- Process 1：`0x0100`，用户态
- Process 2：`0x0180`，内核态

### 1. Process 1 writes to `0xC1B2`

先分解虚拟地址：

- 一级索引：`0x18`
- 二级索引：`0x06`
- 页内偏移：`0x32`

页目录项地址：

$$
0x0100 + 2 \times 0x18 = 0x0130
$$

查内存，`[0x0130] = 0x2101`。

- `P = 1`
- `W = 0`
- `U = 0`
- 页表基址为 `0x2100`

页表项地址：

$$
0x2100 + 2 \times 0x06 = 0x210C
$$

查内存，`[0x210C] = 0x3A47`。

- `P = 1`
- `W = 1`
- `U = 1`

虽然二级页表项存在，但这是用户态写访问，而页目录项的 `U = 0`，因此用户态不能访问，该访问非法。

结论：

- Address of PDE: `0x0130`
- Address of PTE: `0x210C`
- The result of the address translation: `NONE`
- The result of the access: `illegal non-supervisor access`
- Causing entry: `0x0130`

### 2. Process 2 writes to `0x728F`

先分解虚拟地址：

- 一级索引：`0x0E`
- 二级索引：`0x0A`
- 页内偏移：`0x0F`

页目录项地址：

$$
0x0180 + 2 \times 0x0E = 0x019C
$$

查内存，`[0x019C] = 0x1201`。

- `P = 1`
- `W = 0`
- `U = 0`
- 页表基址为 `0x1200`

页表项地址：

$$
0x1200 + 2 \times 0x0A = 0x1214
$$

查内存，`[0x1214] = 0x27C1`。

- `P = 1`
- `W = 0`
- `U = 0`

Process 2 在内核态，`U` 位不限制它，但这次是写操作，而页目录项已经 `W = 0`，所以该页不可写。

结论：

- Address of PDE: `0x019C`
- Address of PTE: `0x1214`
- The result of the address translation: `NONE`
- The result of the access: `page not writable`
- Causing entry: `0x019C`
