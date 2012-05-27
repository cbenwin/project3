/*
 * util.h
 *
 *  Created on: May 26, 2012
 *      Author: Joel
 */

#ifndef UTIL_H_
#define UTIL_H_

typedef enum bool {false = 0, true}bool;

typedef struct SlabInfo{
    int objectSize;
    char* objectStatus;
    bool inUse;
    int objectCount;
} slabInfo;

typedef union ObjectInfo {
	char* tree;
	slabInfo* slabArray;
} objectInfo;

typedef struct Allocator{
    long size;
    void* mem;
    int parm1;
    int* parm2;
    unsigned long flags;
    int handle;
    objectInfo usage;
} allocator;

allocator allocators[512];



bool isPowerOf2(int operand);
void initTree (char* tree, int pages);
void* buddyMemAlloc(allocator* current, long n_bytes);
void printTree (allocator current);
void initArray (char* objectArray, int max);
int getSmallest (int* objectSizes);
int validObjects (int* objectSizes);

#endif /* UTIL_H_ */
