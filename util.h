/*
 * util.h
 *
 *  Created on: May 26, 2012
 *      Author: Joel
 */

#ifndef UTIL_H_
#define UTIL_H_

typedef struct ArrayList{
    int objectSize;
    int* objectStatus;
    int objectCount;
} slab;

typedef struct Allocator{
    long size;
    void* mem;
    int parm1;
    int* parm2;
    unsigned long flags;
    int handle;
    char* tree;
} allocator;

allocator allocators[512];



typedef enum bool {false = 0, true}bool;

bool isPowerOf2(int operand);
void initTree (char* tree, int pages);
void* buddyMemAlloc(allocator* current, long n_bytes);
void printTree (allocator current);
int getSmallest (int* objectSizes);
int validObjects (int* objectSizes);

#endif /* UTIL_H_ */
