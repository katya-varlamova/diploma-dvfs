#pragma once
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

void io_bound();
const int SIZE = 100 * 1024 * 1024;
void mem_bound() ;
void cpu_bound() ;