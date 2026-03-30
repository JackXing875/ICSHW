# ICS作业2答案

邢添珵　2024202862

---

## 1

| value       | Two's complement |
|:---------------|:------|
| 37  | 00100101  |
| -15 | 11110001  | 
| 85 | 01010101 |
| -86 | 10101010 |

---

## 2

| expression       | Binary Representation |
|:---------------|:------|
| us  | 1101  |
| ui | 1100 1100  | 
| us << 1| 1010 |
| i >> 2 | 1111 0011 |
| ui >> 2 | 0011 0011 |
| (short) i | 1100 |
| (int) s | 1111 1101 |

## 3

```c
/ * Determine whether arguments can be added without overflow 
* This function should return 0 if arguments x and y can be added without causing overflow
*/ 
int uadd_ok(unsigned x, unsigned y) {
    unsigned sum = x + y;
    return sum < x;
}
```

附：
![思维痕迹](img/img2.jpg)