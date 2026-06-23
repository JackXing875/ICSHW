# Homework8

姓名：邢添珵
学号：2024202862

## Problem 1

计算机采用 C-SCAN 磁盘调度策略，使用 2KB 的内存空间记录 16384 个磁盘块的空闲状态。

### 1. 

答：

使用 bitmap。

2KB 的空间共有：$2 \times 1024 \times 8 = 16384$ 个 bit，正好可以对应 16384 个磁盘块。可以令第 `i` 个 bit 表示第 `i` 个磁盘块的空闲状态：

- bit = 1：该磁盘块空闲
- bit = 0：该磁盘块已经被使用

分配磁盘块时扫描 bitmap，找到值为 1 的 bit，将其改成 0；释放磁盘块时将对应 bit 改回 1。

### 2

服务顺序为：

```text
120 -> 30 -> 50 -> 90
```

寻道时间为：

$$
(120 - 100) + (120 - 30) + (50 - 30) + (90 - 50) = 170ms
$$

转一圈需要：$\frac{60s}{6000} = 0.01s = 10ms$


一个扇区的传输时间为 0.1ms

4 个请求的旋转等待和传输时间为：$4 \times (5 + 0.1) = 20.4ms$

故总时间为：$170 + 20.4 = 190.4ms$

### 3.

答：

Flash 设备没有机械磁头，不存在寻道时间和旋转等待时间，因此 C-SCAN 没有明显意义。

更有效的方法有：

- 调度目标从减少机械移动变成减少擦除次数、降低写放大、均衡磨损。

## Problem 2

初始状态为：

```text
Log Table: empty

Data Table:
1000 -> 0
2000 -> 4

Flash:
Block 0: a b c d
Block 1: e f g h
Block 2: _ _ _ _
Block 3: _ _ _ _
```

用户依次更新：

```text
e, f, a, a, a, g, g, f, h, e
```

答：

#### 1）写放大

- 用户逻辑写入：10 个 page。
- merge 搬移的额外写入：partial 搬 `g h` 共 2 页，两次 full 各重写 4 页（`a b c d` 与 `e f g h`），合计 `2 + 4 + 4 = 10` 页。

物理写入总量 $10 + 10 = 20 \text{ pages} $

即写放大比为 10 个 pages

#### 2）擦除次数

partial merge 擦 1 个，两次 full merge 各擦 2 个，共 5 个

#### 3）各类 merge 次数

| 类型 | 次数 |
| --- | --- |
| switch merge | 0 |
| partial merge | 1 |
| full merge | 2 |

#### 4）最终状态

```text
Data Table:
1000 -> 12   (Block 3)
2000 -> 4    (Block 1)

Log Table:
e -> 0   (Block 0, page 0)

Flash:
Block 0: e _ _ _    
Block 1: e f g h     
Block 2: _ _ _ _     
Block 3: a b c d     
```



## Problem 3


答：

所有可能输出为：

```text
fd2=4, c=a
fd2=4, child: c=b
fd2=4, c=r
```

或：

```text
fd2=4, c=b
fd2=4, child: c=a
fd2=4, c=r
```

或：

```text
fd2=4, child: c=a
fd2=4, c=b
fd2=4, c=r
```

或：

```text
fd2=4, child: c=a
fd2=4, c=r
fd2=4, c=b
```

或：

```text
fd2=4, child: c=b
fd2=4, c=a
fd2=4, c=r
```

或：

```text
fd2=4, child: c=b
fd2=4, c=r
fd2=4, c=a
```

