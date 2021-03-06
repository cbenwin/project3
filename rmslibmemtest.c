/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
/* DEFINITIONS */
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

typedef struct{
    int size;
    int prevSIZE;
    int handle;
	//linkedlist next;
    bool prevFREED;
	bool freespace1;
} linkedlist;

struct mem{
    unsigned long*allocptr;
    long size;
    int parm1;
    int *parm2;
    unsigned int flags;
    linkedlist LL;
};

/* Global Variables */
struct mem MEMORY[512];
int memALLOCATED = 0;
void *RETVAL;

void memfree (void *region){
    int iter = 0;
    //Iterate over the global array to find the responsible region handler.
    while (&(MEMORY[iter]) != region){
        int size = (int)MEMORY[iter].size;
        int parm1 =(int)MEMORY[iter].parm1;
        int indices = size/parm1;
        if (MEMORY[iter].flags == BUDDY){
            int iter = 0;
            for (; iter < indices; iter++){
                if (region == &MEMORY[iter].allocptr[iter]){
                    
                }
            }
        }
    }
    iter++;
    long curSIZE = MEMORY[iter].size;
    if (MEMORY[iter].flags == BUDDY){
        int lowLEVEL = MEMORY[iter].parm1;
        int numLEV = (MEMORY[iter].size)/lowLEVEL;

    }
}

/* meminit
 * Initializes the memory allocator. 
 * Returns a handle that can be used to identify the allocator.
 */
int meminit (long n_bytes, unsigned int flags, int parm1, int *parm2){
	unsigned long*allocptr;
    //Checking for incorrect value of flags.
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
        int indices = (long)(n_bytes/(pow(2,parm1)));
        int level = 0;
        unsigned long ZERO = 0;
        for (; level < indices; level++){
            allocptr = &ZERO;
            allocptr+1;
        }
    }
	
	//If slab allocation is selected.
	if (flags == SLAB){
		printf("Inside Slab Allocation.\n");
	}
	
	if (flags == FREELIST1){
		//Account for the extra bytes needed for the bitmap
		long extra = (n_bytes/(pow(2,parm1)));
		long extralength = ceil(extra / BYTE);
		//Allocate Memory Structure
		allocptr = malloc(n_bytes + extralength);
	}
	if (flags == FREELIST2){
	    //Account for the extra bytes needed for the bitmap$
        long extra = (n_bytes/(pow(2,parm1)));
        long extralength = ceil(extra / BYTE);
        //Allocate Memory Structure$
        allocptr = malloc(n_bytes + extralength);
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
	return (memALLOCATED - 1);
}
/* getLEVEL(int level, int handle)
 * The level parameter is the current level. The handle is the current MEMORY
 * index. Returns true when it found an appropriate page to allocate.
 * */
void *getLEVEL(int level,int handle){
    int powLEV = (2*level)-1;
    int blah = pow(2,powLEV);
    int curLEV2 = (level-1);
    unsigned int ONE = 1;
    unsigned int TWO = 2;
    struct mem curMEM = MEMORY[handle];
    int curLEV1 = level;
    //while blah and the uint at the current level are unequal
    while (curLEV1 > 0){
        if ((blah & curMEM.allocptr[level])==0){
            if ((powLEV % 2) == 0){
                if ((blah*2)&curMEM.allocptr[level] != 0) break;
            }else {
                if ((blah/2)&curMEM.allocptr[level] != 0) break;
            }
        }
        //Increment through all of the possible values at this level.
        blah = blah/2;
        if (blah == 0) RETVAL = NULL;
        powLEV--;
        curLEV1--;
    }
    //Trying to figure out the block that you need to free/allocate/the
    //concurrent processes of both.
    int log2BLAH = log2(blah);
    unsigned long curVAL = curMEM.allocptr[level];
    //The freeing/allocating of pages.
    if ((log2BLAH % 2) == 0){
        curMEM.allocptr[level] = curVAL ^ blah;
        curMEM.allocptr[level] = curVAL|(curVAL<<ONE);
    }else {
        curMEM.allocptr[level] = curVAL ^ blah;
        curMEM.allocptr[level] = curVAL|(curVAL>>ONE);
    }
    //while you aren't at the highest level of memory
    while (curLEV2 >= 0){
       curMEM.allocptr[curLEV2] = log2BLAH;
       if (blah != 1){
           if ((blah % 2) != 0)  log2BLAH = log2BLAH - 1;
           log2BLAH = log2(log2BLAH);
       }
       curLEV2--;
    }
    RETVAL = &blah;
}

void *memalloc(int handle, long n_bytes){
    struct mem curMEM = MEMORY[handle];
    if (&curMEM == NULL){
        printf ("MEMORY[handle] == NULL.\n");
        return NULL;
    }
    if (curMEM.flags == BUDDY){
        int min = pow(2,curMEM.parm1);
        if (n_bytes < min){
            printf ("You have given an incorrect page size.\n");
            return NULL;
        }
       //current level = largest level
       int level = 0;
       unsigned long blah = 1;
       //size = the size of the entire memory
       int size = (int)curMEM.size;
       int curSIZE = size;
       unsigned int retVAL;
       //if nothing has been allocated in the memory, signified by the highest
       //level being 0
       if (curMEM.allocptr[0] == 0){
            //Try to find the level at which the chunks are equal to n_bytes.
            while ((n_bytes - curSIZE) < 0){
                //Blah increments by powers of 2.
                blah = pow(2,level);
                curSIZE = size / blah;
                //Increment the level of access.
                level++;
             }
             retVAL = level;
             blah = pow(2,level);
             //Set the level of the memory so that a portion of the memory is
             //marked as used.
             //Increment through the levels, by decrementing the level.
             while (level >= 0){
                curMEM.allocptr[level] = blah;
                blah = blah/2;
                level --;
             }
             return &MEMORY[handle].allocptr[retVAL];
       }else {
           //While you aren't at the current level, where the page size equals n_bytes.
           while ((n_bytes - curSIZE) < 0){
               //get the value 2^level, which equals the current number of
               //pages.
               blah = pow(2, level);
               //Get the current pazMEMORY[retVAL].e size, which equals the total memory
               //allocated size divided by the current number of pages.
               curSIZE = size / blah;
               level++;
           }
           if (curMEM.allocptr[level] == 0){
               curMEM.allocptr[level] = pow(2,((2*level)-1));
           }else {
              void *voidRET= getLEVEL(level,handle);
              int curLEVEL = level-1;
              while (voidRET == NULL){
                if (curLEVEL < 0) break;
                voidRET = getLEVEL(curLEVEL,handle);
                curLEVEL--;
             }
             return &MEMORY[handle].allocptr[curLEVEL];
          }
       }
    }
}
int main(){
    int parm1 = 12;
    long region = 65536;
    int parm2 = 0;
    int i;
    int init_test = meminit(region, BUDDY, parm1, &parm2);
    struct mem test = MEMORY[0];
    printf("Test:%p\n", &test);
    printf("Allocptr value: %p\n", test.allocptr);
    printf("N_Bytes: %lu\n", test.size);
    printf("Flags: %d\n", test.flags);
    printf("Parm1: %d\n", test.parm1);
    printf("Parm2: %p\n",test.parm2);
    memalloc(init_test,16384);
    memalloc(init_test,8192);
    memalloc(init_test,8192);
    printf("I'M MAIN\n");
}
