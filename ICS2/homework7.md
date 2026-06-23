# Homework7

姓名：邢添珵
学号：2024202862

## Problem 1

将文件 `a.txt` 映射到两个长度为 100K 的 `char` 型数组上，数组名分别为 `sbuf` 和 `pbuf`，`sbuf` 用共享方式，`pbuf` 用私有方式。假设 page 大小为 4KB。

### 1.

答：

```c
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define ARRAY_LEN (100 * 1024)

int fd = open("a.txt", O_RDWR);

char *sbuf = mmap(NULL, ARRAY_LEN,
                  PROT_READ | PROT_WRITE,
                  MAP_SHARED,
                  fd, 0);

char *pbuf = mmap(NULL, ARRAY_LEN,
                  PROT_READ | PROT_WRITE,
                  MAP_PRIVATE,
                  fd, 0);
```

其中 `sbuf` 使用 `MAP_SHARED`，写入会反映到文件；`pbuf` 使用 `MAP_PRIVATE`，第一次写入某个 page 时会触发 COW。

### 2.

执行代码为：

```c
for (idx = 0; idx < ARRAY_LEN; idx++) {
    sbuf[idx] = sbuf[idx] + 1;
    pbuf[idx] = pbuf[idx] + 1;
}
```

100K 的数组在 4KB page 下共占：$ \frac{100 \times 1024}{4 \times 1024} = 25 $

即 25 个 page。

对每个 page：

- 第一次访问 `sbuf` 时，`sbuf` 对应的页表项还没有建立，会产生 1 次 page fault。
- 第一次访问 `pbuf` 时，`pbuf` 对应的页表项还没有建立，也会产生 1 次 page fault。
- 第一次写 `pbuf` 的该 page 时，因为 `MAP_PRIVATE` 需要 COW，会再产生 1 次 page fault exception。
- `sbuf` 是共享映射，写入本身不需要 COW。

所以总 page fault exception 次数为：$ 25 \times 3 = 75 $

其中 COW 次数为 25

### 3. 

```
sbuf[0] = '1'
sbuf[1] = '1'
pbuf[0] = '2'
pbuf[1] = '1'
```

