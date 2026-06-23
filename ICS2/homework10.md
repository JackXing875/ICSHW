# Homework10

姓名：邢添珵
学号：2024202862

## Problem 1

答：

初始值为

```c
a = 1;
b = 0;
```

填空如下：

```c
void *t1(void *arg) {
    while (1) {
        P(&a);
        printf("b");
        fflush(stdout);
        V(&b);
        V(&b);
        P(&a);
    }
}

void *t2(void *arg) {
    while (1) {
        P(&b);
        printf("e");
        fflush(stdout);
        V(&a);
    }
}
```

## Problem 2


答：

```c
void *doit(void *args) {
    int *base = (int *)args;

    for (int i = 0; i < M; i++) {
        Hash_Insert(&hashtb, base[i]);
    }

    return NULL;
}
```

