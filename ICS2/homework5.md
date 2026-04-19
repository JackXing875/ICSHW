# Homework5

姓名：邢添珵
学号：2024202862

## Problem 1

答：

已知第一页约定 page size 为 64B，故VPO = 6 bit。  
虚拟地址一共 14 bit，因此：

- VPN = 14 - 6 = 8 bit
- TLB 为 4 组，所以 TLB index = 2 bit
- TLB tag = 8 - 2 = 6 bit

0x03FC = 0000 1111 1111 00，则 VPN 为 00001111， VPO 为 111100：

进一步拆分 VPN 得到：TLB tag = 000011 = 0x3 和 TLB index = 11 = 0x3

查 TLB 第 3 组，tag = 03 的表项有效，命中，得到 PPN = 0x0D，所以不会发生 page fault

因此物理地址为：

$$
PA = (PPN << 6) + VPO = (0x0D << 6) + 0x3C = 0x37C
$$
已知 cache 是 16 组、块大小 4B、直接映射，所以：

- Byte offset = 2 bit
- Cache index = 4 bit
- Cache tag = 12 - 4 - 2 = 6 bit

0x37C = 0011 0111 1100，因此：

- Byte offset = 00 = 0x0
- Cache index = 1111 = 0xF
- Cache tag = 001101 = 0xD

查 cache 发现有效位为 0，所以 cache miss。

综上：

| Parameter | Value |
| --- | --- |
| VPN | 0x0F |
| TLB index | 0x3 |
| TLB tag | 0x3 |
| TLB hit?(Y/N) | Y |
| Page fault?(Y/N) | N |
| PPN | 0xD |
| Byte offset | 0x0 |
| Cache index | 0xF |
| Cache tag | 0xD |
| Cache hit?(Y/N) | N |
| Cache byte returned | N/A |
| Physical memory reference | 0x37C |

## Problem 2

答：

已知：

- 虚拟地址空间为 64 位，所以虚拟地址总长度是 64 bit
- page size = 2048B = 2^11 B，所以页内偏移占 11 bit
- 虚拟页号 VPN = 64 - 11 = 53 bit

由于一个页表页的大小是 2048B，所以一页页表中能容纳的页表项数为：

$$
\frac{2^{11}}{2^2} = 2^9 = 512
$$

也就是说，每一级页表最多提供 9 bit 的索引。

要覆盖全部虚拟页号的 53 bit，需要的页表级数 L 满足：

$$
L \ge \frac{53}{9} \approx 5.89
$$

向上取整可得：

$$
L = 6
$$

故需要 6 级页表。

## Problem 3

### 程序涉及到的数据结构被存放在了哪一段？请画图标注。

答：

VA 最高两位标记 segment，因此 1MB 虚拟空间可分为四段，每段大小都是：

$$
\frac{1MB}{4} = 256KB = 2^{18}B
$$

所以 VA 最高 2 bit 为 segment 编号，低 18 bit 为段内偏移

四段对应的 VA 范围为：

- code：0x00000 ~ 0x3FFFF
- data：0x40000 ~ 0x7FFFF
- heap：0x80000 ~ 0xBFFFF
- stack：0xC0000 ~ 0xFFFFF

程序中的数据结构所在段如下：

- main 函数、代码本身、字符串常量 "s=%#X\n" 和 "test.txt" 在 code 段
- 全局变量 a、b 在 data 段
- 局部变量 idx、fd 和局部数组 s[128*1024] 在 stack 段
- 没有使用 heap

画成：

```text
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

### 已知程序输出 s = 0xC1300，求 s 对应的 PA

0xC1300 的最高两位是 11，所以它属于 stack 段。  
段内偏移为：

$$
0xC1300 - 0xC0000 = 0x1300
$$

stack 段参数为：

- base = 1MB = 0x100000
- size = 256KB = 0x40000
- positive = 0

有

$$
PA = 0x100000 - 0x40000 + 0x1300 = 0xC1300
$$

所以 s 对应的物理地址为 0xC1300


### 2. 如果 VA = 258K / 514K，对应的 PA 是多少

#### VA = 258K

$$
258K = 258 \times 1024 = 0x40800
$$

0x40800 最高两位是 01，属于 data 段。  
段内偏移为：

$$
0x40800 - 0x40000 = 0x800
$$

而 data 段大小为：

$$
2KB = 0x800
$$

合法偏移范围应为 0x000 ~ 0x7FF，0x800 已经越界，因此该地址非法，没有对应的物理地址。

结论：VA = 258K 时，PA 不存在，发生越界访问。

#### VA = 514K

$$
514K = 514 \times 1024 = 0x80800
$$

0x80800 最高两位是 10，属于 heap 段。  
段内偏移为：

$$
0x80800 - 0x80000 = 0x800
$$

heap 段大小为 8KB = 0x2000，所以偏移 0x800 合法。  
由于 heap 段 positive = 1，故：

$$
PA = base + offset = 0x700000 + 0x800 = 0x700800
$$

结论：

- VA = 258K：无对应 PA，越界
- VA = 514K：PA = 0x700800

## Problem 4

可知：

- VPO = 6 bit
- 每张页表有 64 / 2 = 32 个表项，所以每级索引都是 5 bit
- 虚拟地址划分为 5 bit 一级索引 + 5 bit 二级索引 + 6 bit 页内偏移

### 1. Process 1 writes to 0xC1B2

- 一级索引：0x18
- 二级索引：0x06
- 页内偏移：0x32

页目录项地址：

$$
0x0100 + 2 \times 0x18 = 0x0130
$$

查内存，[0x0130] = 0x2101。

- P = 1
- 页表基址为 0x2100

页表项地址：

$$
0x2100 + 2 \times 0x06 = 0x210C
$$

查内存，[0x210C] = 0x3A47。

- P = 1
- W = 1
- U = 1

PTE = 0x3A47，低三位为 111，因此该页 present、writable、user accessible。

Process 1 处于用户态，执行写访问，权限满足，故访问成功。

物理页基址为 0x3A40，再加页内偏移 0x32，得到：

$$
PA = 0x3A40 + 0x32 = 0x3A72
$$

因此：

- Address of PDE: 0x0130
- Address of PTE: 0x210C
- The result of the address translation: 0x3A72
- The result of the access: success

### 2. Process 2 writes to 0x728F

分解虚拟地址：

- 一级索引：0x0E
- 二级索引：0x0A
- 页内偏移：0x0F

页目录项地址：

$$
0x0180 + 2 \times 0x0E = 0x019C
$$

查内存，[0x019C] = 0x1201。

- P = 1
- 页表基址为 0x1200

页表项地址：

$$
0x1200 + 2 \times 0x0A = 0x1214
$$

查内存，[0x1214] = 0x27C1。

- P = 1
- W = 0
- U = 0

同样，PDE 只有 Present 位，因此不能从 PDE 判断 W/U 权限。

Process 2 在内核态，U = 0 不会阻止访问；但它执行的是写操作，而 PTE 中 W = 0，因此该页不可写。

故失败原因来自 PTE，而不是 PDE。

结论：

- Address of PDE: 0x019C
- Address of PTE: 0x1214
- The result of the address translation: NONE
- The result of the access: page not writable
- Causing entry: 0x1214
- Causing entry: 0x1214
