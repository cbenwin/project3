/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define FREELIST 0x4
//#define FIRSTFIT 0x0
//#define NEXTFIT 0x08
//#define BESTFIT 0x10
//#define WORSTFIT 0x18
#define BUDDY	0x1
#define SLAB	0x2
#define FREELIST1	0x4|0x00
#define FREELIST2	0x4|0x08
#define FREELIST3	0x4|0x10
#define FREELIST4	0x4|0x18
#define true	1
#define false	0
#define BYTE	8

typedef int bool;

typedef struct linkedlist{
    int size;
    int prevSIZE;
    int handle;
	//linkedlist next;
    bool prevFREED;
	bool freespace1;
} linkedlist;

typedef struct mem{
	void *allocptr;
    unsigned long *mem;
    long size;
    int parm1;
	int *parm2;
    unsigned int flags;
    linkedlist LL;
    int handle;
} mem;

typedef struct{
	unsigned int bit : 1;
} Bit;

struct bit_array {
	unsigned char *array;
	unsigned int numBits;
}; 

/* Global Variables */
mem *MEMORY;
int memALLOCATED = 0;

/* meminit
 * Initializes the memory allocator. 
 * Returns a handle that can be used to identify the allocator.
 */
int meminit (long n_bytes, unsigned int flags, int parm1, int *parm2){
	void *allocptr;
	
    //Checking for incorrect value for flags.
    if (flags != BUDDY && flags != SLAB && flags != FREELIST){
        printf ("Incorrect flag value for meminit.\n");
        return -1;
    }
	
    //If buddy allocation is selected.
    if (flags == BUDDY){
		printf("Inside Buddy Allocation.\n");
		
		//Return -1 if the region size isn't a power of 2.
		if((n_bytes & (n_bytes - 1)) != 0){	//http://forum.codecall.net/topic/63497-determine-power-of-2-in-c/
			fprintf(stderr, "Invalid Size.\n");
			return -1;
		}
		
		//Account for the extra bytes needed for the bitmap
		long extra = (n_bytes/(pow(2,parm1)));
		long extralength = ceil(extra / BYTE);
		
		//Allocate Memory Structure
		allocptr = malloc(n_bytes + extralength);
    }
	
	if (flags == SLAB){
		printf("Inside Slab Allocation.\n");
	}
	
	if (flags == FREELIST1){
	
	}
	
	if (flags == FREELIST2){
	
	}
	
	if (flags == FREELIST3){
	
	}
	
	if (flags == FREELIST4){
	
	}
	
	//Fill out the information in our struct
	MEMORY[memALLOCATED].allocptr = allocptr;
	MEMORY[memALLOCATED].size = n_bytes;
	MEMORY[memALLOCATED].parm1 = parm1;
	MEMORY[memALLOCATED].parm2 = parm2;
	MEMORY[memALLOCATED].flags = flags;
	memALLOCATED++;
	return MEMORY[memALLOCATED].handle;
}


void *memalloc(int handle, long n_bytes){
    if (&MEMORY[handle] == NULL){
        printf ("MEMORY[handle] == NULL.\n");
        return -1;
    }
    int level = 0;
	/*
    int size = MEMORY->param;
    mem curMEM = MEMORY[handle];
    unsigned long *curARRAY = curMEM.mem;
    if (curMEM.flags = BUDDY){
        if (curARRAY[1] == 0){
            while (size != n_bytes){
                level++;
                size *= 2;
            }
        }
    }*/
}

int main (){
    int *nullARR;
    int retINIT = meminit(64000,0x1,4000,nullARR);
    memalloc(retINIT,8000);
}

