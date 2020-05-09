#include "../../include/mem.h"

unsigned long malloc(unsigned long size) {
	//dPartition's Alloc
	if (pMemHandler) return dPartitionAlloc(pMemHandler, size);
	else return 0;
}

unsigned long free(unsigned long start) {
	//dPartition's Free
	if (pMemHandler) return dPartitionFree(pMemHandler, start);
	else return 0;
}

unsigned long kmalloc(unsigned long size) {
	//dPartition's Alloc
	if (pMemHandler) return dPartitionAlloc(pMemHandler, size);
	else return 0;
}

unsigned long kfree(unsigned long start) {
	//dPartition's Free
	if (pMemHandler) return dPartitionFree(pMemHandler, start);
	else return 0;
}