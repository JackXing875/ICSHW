# Homework9

姓名：邢添珵
学号：2024202862

## Problem 1

答：

文件大小为：5MB = 5120KB

每个 data block 为 4KB，因此文件需要： $\frac{5120}{4} = 1280$ 个 data block。

#### 固定大小 data block 方案

一个 indirect block 可以存放：$\frac{4KB}{8B} = \frac{4096}{8} = 512$个指针。

inode 中：

- 12 个 direct pointers 覆盖 12 个 data blocks
- 2 个 indirect pointers 最多覆盖 `2 * 512 = 1024` 个 data blocks
- 剩余 data blocks 数为：$1280 - 12 - 1024 = 244$

double indirect 需要：1 个 double-indirect root block 和 1 个 second-level indirect block

因此元数据空间为：$120 + 8192 + 4096 + 4096 = 16504 B$

### Extent 方式

每条 extent 记录为：$8 + 4 = 12B$

总开销为：$5 \times 12 = 60B$

故：

| 方式 | 元数据空间 |
| --- | --- |
| 固定大小 data block | 16504B |
| extent | 60B |

## Problem 2


### 1. 

答：



原文件有 25 个 data blocks。前 12 个由 direct pointers 指向，后 13 个已经使用第一个 indirect block。

追加写入 $1MB = 256 \times 4KB$ 即需要新分配 256 个 data blocks。

追加后总 data block 数为：$25 + 256 = 281$

由于：$281 < 12 + 1024$

所以仍然只需要第一个 indirect block，不需要分配新的 indirect block。

需要读取的 block：

| 目的 | I/O |
| --- | --- |
| 读 root inode | 1 |
| 读 root directory data block，找到 `foo` | 1 |
| 读 `/foo` inode | 1 |
| 读 `/foo` directory data block，找到 `bar` | 1 |
| 读 `/foo/bar` inode | 1 |
| 读已有 indirect block | 1 |
| 读 data bitmap，寻找空闲 data blocks | 1 |

读 I/O 共 7 

需要写入的 block：

| 目的 | I/O |
| --- | --- |
| 写入 256 个新的 data blocks | 256 |
| 写回 data bitmap | 1 |
| 写回 indirect block，加入 256 个新指针 | 1 |
| 写回 `/foo/bar` inode，更新大小和修改时间 | 1 |
| 写回 `/foo` inode，更新目录时间 | 1 |

写 I/O 共 260

因此总 I/O 次数为 7 + 260 = 267

### 2. 

答：

需要读取的 block：

| 目的 | I/O |
| --- | --- |
| 读 root inode | 1 |
| 读 root directory data block，找到 `foo` | 1 |
| 读 `/foo` inode | 1 |
| 读 `/foo` directory data block，找到 `bar` | 1 |
| 读 `/foo/bar` inode | 1 |

读 I/O 共 5

需要写入的 block：

| 目的 | I/O |
| --- | --- |
| 写回 `/foo` directory data block，把 `bar` 改为 `bar2` | 1 |
| 写回 `/foo` inode，更新目录修改时间 | 1 |
| 写回 `/foo/bar` inode，更新文件元数据时间 | 1 |

写 I/O 共 3

因此总 I/O 次数为 8
