#include <stdlib.h>
#include <stdio.h>

int main() {
    int *data = (int *)malloc(sizeof(int) * 100);
    data[100] = 0;
    free(data);
    return 0;
}