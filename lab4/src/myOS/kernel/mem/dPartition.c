#include "../../include/myPrintk.h"


//dPartition 是整个动态分区内存的数据结构
struct dPartition{
	unsigned long size;
	unsigned long firstFreeStart; 
};

void showdPartition(struct dPartition *dp){
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}


// EMB每一个block的数据结构，userdata可以暂时不用管。
struct EMB{
	unsigned long size;
	union {
		unsigned long nextStart;    // if free: pointer to next block
        unsigned long userData;		// if allocated, blongs to user
	};	                           
};

void showEMB(struct EMB * emb){
	myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x)\n", emb, emb->size, emb->nextStart);
}

unsigned long dPartitionInit(unsigned long start, unsigned long totalSize){
	//本函数需要实现！！！
	/*
	初始化内存。
	在地址start处，首先是要有dP结构体表示整个数据结构。
	然后，一整块的EMB被分配（以后使用内存会逐渐拆分），在内存中紧紧跟在dP后面，然后dP的firstFreeStart指向EMB。
	返回start首地址。
	注意有两个地方的大小问题，第一个是由于内存肯定要有一个EMB和一个dP，totalSize肯定要比这两个加起来大不能太小。
	第二个注意EMB的可以空间不是totalsize，因为dP要占空间
	*/
	if (totalSize < sizeof(struct EMB) + sizeof(struct dPartition) )
		return 0;

	struct dPartition* dp = (struct dPartition*) start;
	dp->size = totalSize;
	dp->firstFreeStart = start + 8;

	struct EMB* emb = (struct EMB*)(dp->firstFreeStart);
	emb->size = totalSize - 16;
	emb->nextStart = 0;
	return start;

}

void dPartitionWalkByAddr(unsigned long dp){
	//本函数需要实现！！！
	/*本函数遍历输出EMB 方便调试
	先打印dP的信息，可调用上面的showdPartition。
	然后按地址的大小遍历EMB，对于每一个EMB，可以调用上面的showEMB输出其信息
	*/
	struct dPartition* dPart = (struct dPartition*) dp;
	struct EMB* emb = (struct EMB*)  dPart->firstFreeStart;

	showdPartition(dPart);

	while (emb) {
		showEMB(emb);
		emb = (struct EMB*) emb->nextStart;
	}

}

//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
**/
unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size){
	//本函数需要实现！！！
	/*
	使用firstfit的算法分配空间，当然也可以使用其他fit，不限制。
	最后，成功分配返回首地址，不成功返回0
	注意的地方：
	1、和eFPartition一样，要注意实际分配的block的大小和传入的参数size是不一样的。要注意字节对齐的问题，还有它最小值不是0，因为EMB数据结构有大小。在比较所需要的大小和block的size时候，也用的是实际分配需要的大小而非传入的参数size。
	2、分配就好像是链表的一个块分裂成两个，注意边界情况。
	*/
	//illegal size
	if (size <= 0) return 0;

	struct dPartition* dPart = (struct dPartition*) dp;
	struct EMB* pre = 0;
	struct EMB* emb = (struct EMB*) dPart->firstFreeStart;

	size += 4;
	if ((size & 7) != 0)
		size = size - (size & 7) + 8;

	while (emb) {
		if (emb->size >= size) {
			if (emb->size - size >= 16) {

				struct EMB* next = (struct EMB*) ((unsigned long)emb + size + 8);
				next->size = emb->size - size - 8;
				next->nextStart = emb->nextStart;
				emb->size = size;//update current emb

				if (pre == 0) dPart->firstFreeStart = (unsigned long)next;
				else pre->nextStart = (unsigned long)next;

				return (unsigned long)emb;

			}
			else {
				if (pre == 0) dPart->firstFreeStart = (unsigned long)emb->nextStart;
				else pre->nextStart = (unsigned long)emb->nextStart;

				return (unsigned long)emb;
			}
		}
		pre = emb;
		emb = (struct EMB*) emb->nextStart;
	}

	return 0;
}

/*
 *r
 */
unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){
	//本函数需要实现！！！
	/*按照对应的fit的算法释放空间
	注意检查要释放的start~end这个范围是否在dp有效分配范围内
	返回1 没问题
	返回0 error
	*/

	struct dPartition* dPart = (struct dPartition*) dp;
	struct EMB* emb = (struct EMB*) dPart->firstFreeStart;
	struct EMB* tar = (struct EMB*) start;

	//不在范围内
	if (start < dp + 8 || start >= dp + dPart->size) {
		myPrintk(0xf, "error\n");
		return 0;
	}


	unsigned long beg = (unsigned long)tar;
	unsigned long end = (unsigned long)tar + 8 + tar->size;

	struct EMB* pre, * next;
	pre = next = 0;


	while (emb) {
		if ((unsigned long)emb < (unsigned long)tar) pre = emb;
		else if ((unsigned long)emb > (unsigned long) tar) {
			next = emb;
			break;
		}
		emb = (struct EMB*) emb->nextStart;
	}

	if (next) {
		if (end == (unsigned long)next) {
			tar->nextStart = next->nextStart;
			tar->size += next->size;
		}
		else tar->nextStart = (unsigned long)next;
	}
	else tar->nextStart = 0;

	if (pre) {
		if (beg == (unsigned long)pre + 8 + pre->size) {
			pre->nextStart = tar->nextStart;
			pre->size += tar->size;
		}
		else pre->nextStart = (unsigned long)tar;
	}
	else dPart->firstFreeStart = (unsigned long)tar;

	//free succeed
	return 1;
	
}

//wrap: we select firstFit, you can select another one
//user need not know this
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long	 dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}
