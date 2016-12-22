#include "memory.h"
#include <stdlib.h>
#include <string.h>

void *MemoryExpand(MemorySection originMemory, size_t newMemorySize) {
	if (!realloc(originMemory.address, newMemorySize)) {
		void *newBuffer = malloc(newMemorySize);
		memcpy(newBuffer, originMemory.address, originMemory.size);
		
		free(originMemory.address);
		return newBuffer;
	}
	return originMemory.address;
}