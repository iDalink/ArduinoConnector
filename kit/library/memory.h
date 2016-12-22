#ifndef __LIBRARY_MEMORY__
#define __LIBRARY_MEMORY__

#include <stdio.h>

typedef struct {
	void *address;			// 内存地址
	size_t size;			// 内存大小
} MemorySection;

/**
 * 扩展内存
 * @param originMemory 原始内存
 * @param newMemorySize 目标内存大小
 * @return 扩展后的内存地址
 */
void *MemoryExpand(MemorySection originMemory, size_t newMemorySize);


#endif