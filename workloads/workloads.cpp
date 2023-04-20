#include "workloads.h"
void io_bound()
{
    FILE *f;
    f = fopen("cats_big.mp4","rb+");
    char byte[1];
    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    for (int i = 0; i < 1000000; i++) {
        fseek(f, rand() % size, SEEK_SET);
        fread(byte,sizeof(char), 1, f);
        byte[0] &= rand() % 256;
        fwrite(byte,sizeof(char), 1, f);
    }
    fclose(f);
}
void mem_bound() {
    int *arr = (int *) malloc(SIZE * sizeof(int));
    for (int j = 0; j < 100; j++) {
        for (long i = 0; i < 2000000; i++) {
            arr[rand() % SIZE] += 5389;
        }
    }
    free(arr);
}
void cpu_bound() {
    register long i asm ("rax");
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
    for (i = 0; i < 200000000; i++);
}