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