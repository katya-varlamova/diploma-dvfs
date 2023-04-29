#include <cstdlib>
#include <cmath>
int main()
{
    const long bufSize = 1 * 1024 * 1024 * 1024;
    int cpuPortion = 5;
    long cpuIters = 100000000 * std::pow(1.5, 0);
    long memIters = 100000000; // 5000; 5000 * 20000
    int *arr = (int *) malloc(bufSize * sizeof(int));
    long* rands = (long *) malloc(memIters * sizeof (long));
    for (long i = 0; i < memIters; i++) {
        int r = rand();
        rands[i] = (i * 12 * 1024 * 1024 + (r % 12 * 1024 * 1024)) % bufSize;
    }
    register long i asm ("rax") = 0;
    // mem-bound part
    for (; i < memIters; i++) ++arr[rands[i]];
    // cpu-bound part
    //for (; i < cpuIters; i++);
}

//void io_bound()
//{
//    FILE *f;
//    f = fopen("cats_big.mp4","rb+");
//    char byte[1];
//    fseek(f, 0L, SEEK_END);
//    long size = ftell(f);
//    for (int i = 0; i < 1000000; i++) {
//        fseek(f, rand() % size, SEEK_SET);
//        fread(byte,sizeof(char), 1, f);
//        byte[0] &= rand() % 256;
//        fwrite(byte,sizeof(char), 1, f);
//    }
//    fclose(f);
//}
