# Homework3

姓名：邢添珵

学号：2024202862

### Problem 1
First, write a simple program called null.c that creates a pointer to an integer, sets it to NULL, and then tries to dereference it. Compile this into an executable called null. What happens when you run this program? 

```c
#include <stdio.h>

int main() {
    int *ptr = NULL;
    printf("%d", *ptr);
    return 0;
}
```

- Segmentation fault (core dumped)


### Problem 2
Next, compile this program with symbol information included (with the-g flag). Doing so let’s put more information into the executable, enabling the debugger to access more useful information about variable names and the like. Run the program under the debugger by typing gdb null and then, once gdb is running, typing run. What does gdb show you? 

```bash
Program received signal SIGSEGV, Segmentation fault.
0x0000555555555161 in main () at null.c:5
5           printf("%d", *ptr);
```

### Problem 3

Finally, use the valgrind tool on this program. We’ll use the memcheck tool that is a part of valgrind to analyze what happens. Run this by typing in the following: valgrind --leak-check=yes null. What happens when you run this? Can you interpret the output from the tool? 

- output:
```bash
==22656== Memcheck, a memory error detector
==22656== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==22656== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==22656== Command: ./null
==22656== 
==22656== Invalid read of size 4
==22656==    at 0x109161: main (null.c:5)
==22656==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==22656== 
==22656== 
==22656== Process terminating with default action of signal 11 (SIGSEGV)
==22656==  Access not within mapped region at address 0x0
==22656==    at 0x109161: main (null.c:5)
==22656==  If you believe this happened as a result of a stack
==22656==  overflow in your program's main thread (unlikely but
==22656==  possible), you can try to increase the size of the
==22656==  main thread stack using the --main-stacksize= flag.
==22656==  The main thread stack size used in this run was 8388608.
==22656== 
==22656== HEAP SUMMARY:
==22656==     in use at exit: 0 bytes in 0 blocks
==22656==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==22656== 
==22656== All heap blocks were freed -- no leaks are possible
==22656== 
==22656== For lists of detected and suppressed errors, rerun with: -s
==22656== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
Segmentation fault (core dumped)
```

- 解释：`0x109161` 处程序试图读一个4字节大小的数据，但操作是非法的。地址 `0x0` 既不在栈上，也不是 *malloc* 分配的，也不是最近释放的内存，完全与当前程序无关。总之，程序因为 *segmentation fault* 而终止。

### Problem 4
Write a simple program that allocates memory using malloc() but forgets to free it before exiting. What happens when this program runs? Can you use gdb to find any problems with it? How about valgrind (again with the --leak-check=yes flag)? 


```c
#include <stdio.h>

int main() {
    int *p = (int *)malloc(sizeof(int) * 10);
    return 0;
}
```
- 程序正常运行
- 用 `gdb` 调试，发现 `[Inferior 1 (process 14609) exited normally]` ，没有检测到问题
- 使用 `valgring`

```bash
==15807== Memcheck, a memory error detector
==15807== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==15807== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==15807== Command: ./malloc
==15807== 
==15807== 
==15807== HEAP SUMMARY:
==15807==     in use at exit: 40 bytes in 1 blocks
==15807==   total heap usage: 1 allocs, 0 frees, 40 bytes allocated
==15807== 
==15807== 40 bytes in 1 blocks are definitely lost in loss record 1 of 1
==15807==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==15807==    by 0x10915E: main (malloc.c:4)
==15807== 
==15807== LEAK SUMMARY:
==15807==    definitely lost: 40 bytes in 1 blocks
==15807==    indirectly lost: 0 bytes in 0 blocks
==15807==      possibly lost: 0 bytes in 0 blocks
==15807==    still reachable: 0 bytes in 0 blocks
==15807==         suppressed: 0 bytes in 0 blocks
==15807== 
==15807== For lists of detected and suppressed errors, rerun with: -s
==15807== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

发现报告 `40 bytes in 1 blocks are definitely lost in loss record 1 of 1` ，也就是说内存泄漏了

### Problem 5
Write a program that creates an array of integers called data of size 100 using malloc; then, set data[100] to zero. What happens when you run this program? What happens when you run this program using valgrind? Is the program correct? 

```c
#include <stdlib.h>
#include <stdio.h>

int main() {
    int *data = (int *)malloc(sizeof(int) * 100);
    data[100] = 0;
    free(data);
    return 0;
}
```

- 程序可以正常运行

- 使用 `valgrind`

```bash
==5142== Memcheck, a memory error detector
==5142== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==5142== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==5142== Command: ./leak
==5142== 
==5142== Invalid write of size 4
==5142==    at 0x10918D: main (leak.c:6)
==5142==  Address 0x4a851d0 is 0 bytes after a block of size 400 alloc'd
==5142==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==5142==    by 0x10917E: main (leak.c:5)
==5142== 
==5142== 
==5142== HEAP SUMMARY:
==5142==     in use at exit: 0 bytes in 0 blocks
==5142==   total heap usage: 1 allocs, 1 frees, 400 bytes allocated
==5142== 
==5142== All heap blocks were freed -- no leaks are possible
==5142== 
==5142== For lists of detected and suppressed errors, rerun with: -s
==5142== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

提示有 ```Invalid write of size 4``` ，即我们的越界访问行为

- 显然不正确，我们越界访问了


### Problem 6

Create a program that allocates an array of integers (as above), frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use valgrind on it? 

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *data = (int *)malloc(100 * sizeof(int));
    data[0] = 10;
    free(data); 
    printf("%d\n", data[0]); 
    return 0;
}
```

- 程序可以运行，我运行了三次，分别输出结果 `-1441719620` , `14871951` 和 `323070625` 

- 使用 `valgrind`

```bash
==10412== Memcheck, a memory error detector
==10412== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==10412== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==10412== Command: ./uaf
==10412== 
==10412== Invalid read of size 4
==10412==    at 0x1091BD: main (uaf.c:10)
==10412==  Address 0x4a85040 is 0 bytes inside a block of size 400 free'd
==10412==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==10412==    by 0x1091B8: main (uaf.c:8)
==10412==  Block was alloc'd at
==10412==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==10412==    by 0x10919E: main (uaf.c:5)
==10412== 
10
==10412== 
==10412== HEAP SUMMARY:
==10412==     in use at exit: 0 bytes in 0 blocks
==10412==   total heap usage: 2 allocs, 2 frees, 1,424 bytes allocated
==10412== 
==10412== All heap blocks were freed -- no leaks are possible
==10412== 
==10412== For lists of detected and suppressed errors, rerun with: -s
==10412== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
提示 `Invalid read of size 4 at 0x1091BD: main (uaf.c:10)`

### Problem 7

Now pass a funny value to free (e.g., a pointer in the middle of the array you allocated above). What happens? Do you need tools to find this type of problem? 

```c
#include <stdlib.h>

int main() {
    int *data = (int *)malloc(100 * sizeof(int));
    free(&data[50]);
    
    return 0;
}
```

- 发现程序报错 `free(): invalid pointer    Aborted (core dumped)`
- `gdb` 和 `valgrind` 可以找到这个错误

### Problem 8

Try out some of the other interfaces to memory allocation. For example, create a simple vector-like data structure and related routines that use realloc() to manage the vector. Use an array to store the vectors elements; when a user adds an entry to the vector, use realloc() to allocate more space for it. How well does such a vector perform? How does it compare to a linked list? Use valgrind to help you find bugs. 

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;     
    int capacity;  
} Vector;

void init_vector(Vector *v) {
    v->size = 0;
    v->capacity = 2; 
    v->data = (int *)malloc(v->capacity * sizeof(int));
}

void push_back(Vector *v, int value) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = (int *)realloc(v->data, v->capacity * sizeof(int));
    }
    v->data[v->size] = value;
    v->size++;
}

void free_vector(Vector *v) {
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

int main() {
    Vector v;
    init_vector(&v);
    
    for (int i = 0; i < 10; i++) {
        push_back(&v, i);
        printf("Added %d, Capacity: %d\n", i, v.capacity);
    }
    
    free_vector(&v);
    return 0;
}
```

-  性能分析：
    * **增：** 大多数情况下是 $O(1)$ 的时间复杂度。只有在容量满时触发 `realloc` 才是 $O(n)$
    * **查：** 支持通过索引 `v->data[i]` 随机访问，时间复杂度 $O(1)$。
2.  与链表对比：
    * 链表不需要 `realloc` 拷贝数据，每次插入都是真正的 $O(1)$，但它不支持随机访问。
    * 链表每个节点都需要额外的指针空间，且内存分散在堆区各处，缓存命中率低，实际遍历速度不如 Vector。


- 使用 Valgrind：

```bash
==12066== Memcheck, a memory error detector
==12066== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==12066== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==12066== Command: ./vector
==12066== 
Added 0, Capacity: 2
Added 1, Capacity: 2
Added 2, Capacity: 4
Added 3, Capacity: 4
Added 4, Capacity: 8
Added 5, Capacity: 8
Added 6, Capacity: 8
Added 7, Capacity: 8
Added 8, Capacity: 16
Added 9, Capacity: 16
==12066== 
==12066== HEAP SUMMARY:
==12066==     in use at exit: 0 bytes in 0 blocks
==12066==   total heap usage: 5 allocs, 5 frees, 1,144 bytes allocated
==12066== 
==12066== All heap blocks were freed -- no leaks are possible
==12066== 
==12066== For lists of detected and suppressed errors, rerun with: -s
==12066== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

说明没有问题