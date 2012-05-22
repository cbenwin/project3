#include <stdio.h>
#include <stdlib.h>
#define FREELIST 0x4
#define FIRSTFIT 0x0
#define NEXTFIT 0x08
#define BESTFIT 0x10
#define WORSTFIT 0x18
#define BUDDY   0x1
#define SLAB    0x2
typedef mem{
    unsigned long *mem;
    long size;
    int param;
    unsigned long flags;
    linkedlist LL;
    int handle;
} mem;
typedef linkedlist{
    bool freespace = true;
    int size;
    linkedlist next;
    int prevSIZE;
    int handle;
    bool prevFREED = false;
} linkedlist;
mem *MEMORY;
int memALLOCATED = 0;
int main (){
    int *nullARR;
    int retINIT = meminit(64000,0x1,4000,nullARR);
    memalloc(retINIT,8000);
}
int meminit (long n_bytes, unsigned int flags, int parm1, int *parm2){
    //Checkng for incorrect value for flags.
    if (flags != BUDDY && flags != SLAB && flags != FREELIST){
        printf ("Incorrect flag value for meminit.\n");
        return -1;
    }
    //If using buddy allocation.
    if (flags == BUDDY){
        //If incorrect values for n_bytes or parm1
        if ((parm1 > n_bytes)||(n_bytes % 2 != 0)||(parm1 % 2 != 0)){ 
            printf("Incorrect parm1 or n_bytes value for buddy allocation.\n");
            return -1;
        }
        MEMORY[memALLOCATED].handle = memALLOCATED;
        long length =(long) (n_bytes/parm1);
        MEMORY[memALLOCATED].size = length;
        MEMORY[memALLOCATED].flags = flags;
        MEMORY[memALLOCATED].mem = (unsigned long*)malloc(sizeof(unsigned long)*length);
        memALLOCATED++;
        return MEMORY[memALLOCATED].handle;
    }
}
void *memalloc(int handle, long n_bytes){
    if (MEMORY[handle] == NULL){
        printf ("MEMORY[handle] == NULL.\n");
        return -1;
    }
    int level = 0;
    int size = param;
    mem curMEM = MEMORY[handle];
    unsigned long *curARRAY = curMEM.mem;
    if (curMEM.flags = BUDDY){
        if (curARRAY[1] == 0){
            while (size != n_bytes){
                level++;
                size *= 2;
            }
        }
    }
}
