#include "PortionWorkload.h"

PortionWorkload::PortionWorkload(double cpuPortion) {
    m_portion = cpuPortion;
    cpuIters = 100000 * std::pow(1.5, cpuPortion * 25);
    memIters = 100000; // 5000; 5000 * 20000
//       if (cpuPortion < 0.51) {
//           totalIters = 50000000;
//           memIters = totalIters * (cpuPortion * (-0.9977) / 0.5 + 1);
//       }
//       else if (cpuPortion < 0.99) {
//           totalIters = 500000000;
//           memIters = totalIters / 200 / std::pow(1.5, 2 * (cpuPortion - 0.5) * 10);
//       }
//       else {
//           totalIters = 500000000;
//           memIters = 0;
//       }
//        cpuIters = totalIters;
    arr = (int *) malloc(bufSize * sizeof(int));
    rands = (long *) malloc(memIters * sizeof (long));
    for (int i = 0; i < memIters; i++) {
        int r = rand();
        rands[i] = (i * cacheSize + (r % cacheSize)) % bufSize;
    }
}
void PortionWorkload::RunWorkload() {
    register long i asm ("rax") = 0;
    // mem-bound part
    for (; i < memIters; i++) ++arr[rands[i]];
    // cpu-bound part
    for (; i < cpuIters; i++);
}

int PortionWorkload::GetDescriptor() {
    return (int)(m_portion * 100);
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
