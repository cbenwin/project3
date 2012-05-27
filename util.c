#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

bool isPowerOf2(int operand) {
	int exponent = 1;
	int power = pow(2,exponent);

	while (operand >= power) {
		if (operand == power)
			return true;
		power = pow(2, ++exponent);
	}
	return false;
}

void initTree (char* tree, int pages) {
	int max = (2 * pages) - 1;
	for (int index = 0; index < max; ++index) {
		tree[index] = 0;
	}
}

void printTree (allocator current) {

	const long minPageSize = pow(2, current.parm1);
	const int pages = current.size/minPageSize;
	const int nodes = (2 * pages) - 1;
	const int levels = (log(nodes)/log(2));

	for (int index = 0; index <= levels; ++ index) {
		int startIndex = pow(2, index) - 1;
		int levelNodes = pow(2, index);

		printf("**%s %d: ", "Level", index);
		fflush(stdout);
		while (levelNodes > 0) {
			printf("%d", current.tree[startIndex]);
			fflush(stdout);
			++startIndex;
			--levelNodes;
		}
		printf("\n");
		fflush(stdout);
	}
}

int findBuddyHelper (char* tree, int myLevel, int* buddyLevel) {

	if (myLevel == 0) {
		if (tree[0] == 1)
			return -1;
		else {
			tree[0] = 1;
			printf("%s: %d\n", "Tree of 0", tree[0]);
			fflush(stdout);
			*buddyLevel = myLevel;
			printf("%s: %p\n", "Node Address", &tree[0]);
			fflush(stdout);
			return 0;
		}
	}

	//in the array, nodes of the level will start at index 2^myLevel - 1
	int startIndex = pow(2, myLevel) - 1;

	//number of nodes corresponding with myLevel
	int levelNodes = pow(2, myLevel);

	//if there are any chunks of the requested size that have open buddies,
	//mark the chunk as used & return address
	for (int index = 0; index < levelNodes; index += 2) {
		int leftNode = startIndex + index;
		int rightNode = startIndex + index + 1;
		if (tree[leftNode] == 1 && tree[rightNode] == 0) {
			*buddyLevel = myLevel;
//			printf("%s: %p\n", "Node Address", &tree[rightNode]);
//			fflush(stdout);
			return rightNode;
		}
		if (tree[rightNode] == 1 && tree[leftNode] == 0) {
			*buddyLevel = myLevel;
//			printf("%s: %p\n", "Node Address", &tree[leftNode]);
//			fflush(stdout);
			return leftNode;
		}
	}

//	printf("**%s %d**\n", "No buddies on level", myLevel);
//	fflush(stdout);

	--myLevel;
	return findBuddyHelper (tree, myLevel, buddyLevel);
}

void* findBuddy (char* tree, int myLevel) {

	int buddyLevel = myLevel;
	int buddyIndex = findBuddyHelper (tree, myLevel, &buddyLevel);

	//if no free buddies were found
	if(buddyIndex < 0)
		return 0;

	tree[buddyIndex] = 1;
	++buddyLevel;

	//if the free buddy is found, if it is on the same level, set to 1 and return address
	//if on another level, set all parent nodes to 1 and return address
	while (buddyLevel <= myLevel) {
		buddyIndex = (buddyIndex * 2) + 1;
		tree[buddyIndex] = 1;
		++buddyLevel;
	}
	return &tree[buddyIndex];
}

//finds the level in the tree where the current chunk of memory must be placed
int findLevel(const long n_bytes, long size, const long pageSize) {
	if (size <= pageSize)
		return 0;

	if (n_bytes <= size/2)
		return (1 + findLevel(n_bytes, size/2, pageSize));
	return 0;
}

void* buddyMemAlloc(allocator* current, const long n_bytes) {

	const long size = current->size;
	char* tree = current->tree;
	const long minPageSize = pow(2, current->parm1);

	int myLevel = findLevel (n_bytes, size, minPageSize);
//	printf("%s: %d\n", "MyLevel", myLevel);
//	fflush(stdout);

	void* buddy = findBuddy(tree, myLevel);

//	printf("%s: %p\n", "Buddy Address", buddy);
//	fflush(stdout);
	return buddy;
}

int getSmallest (int* objectSizes) {

	if (objectSizes == NULL)
		return -1;

	int objectIndex = 0;
	int smallest = -1;

	while (objectSizes[objectIndex] != 0) {

		int current = objectSizes[objectIndex];

		//if first time through the loop, set to first entry in array
		if (smallest < 0) {
			smallest = current;
			++objectIndex;
			continue;
		}

		//if current entry is smaller than previous "smallest", replace previous entry
		if (current < smallest) {
			smallest = current;
			++objectIndex;
		}
	}

	return smallest;
}

//returns # of valid object sizes in int* object Sizes
//returns -1 if invalid object size found
int validObjects (int* objectSizes) {

	if (objectSizes == NULL)
		return -1;

	int objectIndex = 0;

	while (objectSizes[objectIndex] != 0) {

		int current = objectSizes[objectIndex];

		//check to see that all object sizes are power of 2
		if (!isPowerOf2(current))
			return -1;
		++objectIndex;
	}

	return objectIndex;
}
