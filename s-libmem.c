#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

#define FREELIST 0x4
#define FIRSTFIT 0x0
#define NEXTFIT 0x08
#define BESTFIT 0x10
#define WORSTFIT 0x18
#define BUDDY   0x1
#define SLAB    0x2

int allocatorIndex = 0;
int meminit (long n_bytes, unsigned int flags, int parm1, int *parm2);
void *memalloc(int handle, long n_bytes);
void memfree (void* region);


int main (){
	meminit(1024, BUDDY, 2, 0);
	memalloc (0, 4);
	memalloc (0, 12);
	memalloc (0, 32);
	memalloc (0, 4);
	memalloc (0, 64);
	memalloc (0, 5);
	memalloc (0, 44);
	memalloc (0, 16);
	memalloc (0, 120);
	printTree(allocators[0]);
}


/**
 * parameters:
 * 		n_bytes - number of bytes that this allocator should manage
 * 		flags - describe the behavior of the allocator
 * 		parm1 & parm2 - used to customize a memory allocator
 *
 * 	return value:
 * 		int used to identify the allocator, errors indicated by negative numbers
 */
int meminit (long n_bytes, unsigned int flags, int parm1, int *parm2){
    //if number of allocators exceeds max number
	if (allocatorIndex >= 512) {
    	fprintf(stderr, "Meminit: The maximum number of allocators have been created.\n");
    	return -1;
    }

	//Checking for incorrect value for flags.
    if (flags != BUDDY && flags != SLAB && flags != FREELIST){
        fprintf (stderr, "Meminit: Incorrect flag value.\n");
        return -1;
    }

	//If memory region is not a power of 2
	if (!(isPowerOf2(n_bytes))){
		printf("Memory region must be a power of 2.\n");
		fflush (stdout);
		return -1;
	}

    allocator* current = &allocators[allocatorIndex];
    current->size = n_bytes;
	current->flags = flags;
	current->handle = allocatorIndex;

    switch(flags) {
    	//If buddy allocation specified. parm1 = min page size in address bits (12 means 2^12 byte page size)
    	case(BUDDY): {

			//error if page size is larger that 2^31
        	if (parm1 > 30){
    			fprintf(stderr, "Page size must be smaller than 2^31.\n");
    			return -1;
    		}

    		const int minPageSize = pow(2, parm1);

			//error if minimum page size (param1) is larger than the total allocated memory (n_bytes)
			if (minPageSize > n_bytes) {
				fprintf(stderr, "Minimum page size is larger than memory region.\n");
				return -1;
			}

    		//max number of pages for memory region
    		const int pages = n_bytes/minPageSize;
			const int nodes = (2 * pages) - 1;

			//allocate number of bytes for memory region and overhead
			//for tree with number of nodes necessary
    		current->mem = malloc(n_bytes + (sizeof (char) * nodes));

    		//assigning address after memory allocated for buddy to tree array
    		current->tree = (current->mem + n_bytes);

    		//initializing tree array
    		initTree (current->tree, pages);
			current->parm1 = parm1;

			break;
    	}

    	case(SLAB): {

    		//minimum page size set to 4k
    		const int minPageSize = 4096;
    		const int slabSize = (parm1 * 4096);

    		if (minPageSize > n_bytes) {
				fprintf(stderr, "Minimum page size is larger than memory region.\n");
				return -1;
			}

    		if (slabSize > n_bytes) {
    			fprintf(stderr, "Minimum slab size is larger than memory region.\n");
    			return -1;
    		}

    		current->parm1 = parm1;
    		current->parm2 = parm2;
    		const int smallest = getSmallest (parm2);

    		if (smallest > slabSize) {
    			fprintf(stderr, "Minimum object size is larger than slab size.\n");
    			return -1;
    		}

    		//returns number of valid object sizes, -1 if there is an invalid object size
    		int objects = validObjects(parm2);
    		if (objects < 0) {
    			fprintf(stderr, "Error: Invalid object size.\n");
    			return -1;
    		}

    		//number of slabs in current memory region
    		int slabCount = (n_bytes / slabSize);
    		//maximum number of objects per slab (if smalles object size)
    		int maxObjects = (slabSize / smallest);

    		current->mem = malloc(n_bytes);
    		slab slabs[slabCount];

			break;
    	}

    	case(FREELIST):
    		break;

    	default:
    		return -1;
    }

    allocatorIndex++;
    return current->handle;
}


void* memalloc(int handle, long n_bytes){

	//if handle is past the current allocator index, handle is invalid.
	if (handle > allocatorIndex){
        printf ("The specified memory manager identifier is invalid.\n");
        return 0;
    }

	//set current to referenced allocator;
	allocator* current = &allocators[handle];

	if (n_bytes > current->size) {
		printf ("Handle %d: total mem %ld: requested mem %ld: memory region not large enough.\n",
				current->handle, current->size, n_bytes);
		fflush (stdout);
		return 0;
	}

    switch(current->flags) {
    	case (BUDDY): {
    		void* buddy = buddyMemAlloc(current, n_bytes);
    		if (buddy == 0) {
    			printf ("%s %d: %s %ld bytes\n", "Handle", handle,
    					"Unable to allocate", n_bytes);
    		}
    		break;
    	}
    }
    return 0;
}


void memfree (void* region) {

}
