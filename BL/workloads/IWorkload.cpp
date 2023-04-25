#include "IWorkload.h"
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
