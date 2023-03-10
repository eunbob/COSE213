#include <stdio.h>
#include <stdlib.h> // malloc, realloc

#include "adt_heap.h"

/*typedef struct
{
	void **heapArr;
	int	last;
	int	capacity;
	int (*compare) (void *arg1, void *arg2);
} HEAP;*/


/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index)
{
	int parent;
	void* p;
	void* c;
	if(index > 0)
	{
		parent = (index - 1) / 2;
		p = (heap->heapArr)[parent];
		c = (heap->heapArr)[index];
		
		if(heap->compare(p, c) < 0)
		{
			(heap->heapArr)[parent] = c;
			(heap->heapArr)[index] = p;
			_reheapUp(heap, parent);
		}
	}
}

/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index)
{
	int lcidx, rcidx, large;
	void* rc;
	void* lc;
	void* temp;
	lcidx = index * 2 + 1;
	rcidx = index * 2 + 2;
	
	if(lcidx <= heap->last)
	{
		lc = (heap->heapArr)[lcidx];
		if(rcidx <= heap->last)
		{
			rc = (heap->heapArr)[rcidx];
			large = (heap->compare(lc, rc) >= 0) ? lcidx : rcidx;
		}
		
		else
		{
			large = lcidx;
		}
		
		if(heap->compare(heap->heapArr[index], heap->heapArr[large]) < 0)
		{
			temp = (heap->heapArr)[index];
			(heap->heapArr)[index] = (heap->heapArr)[large];
			(heap->heapArr)[large] = temp;
			_reheapDown(heap, large);
		}
	}
	
}



/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heap_Create( int capacity, int (*compare) (void *arg1, void *arg2))
{
	HEAP* heap;
	heap = (HEAP*)malloc(sizeof(HEAP));
	
	void** heapArr;
	heapArr = (void**)malloc(sizeof(void*) * capacity);
	
	heap->heapArr = heapArr;
	heap->last = -1;
	heap->capacity = capacity;
	heap->compare = compare;
	
	return heap;
}

/* Free memory for heap
*/
void heap_Destroy( HEAP *heap)
{
	free(heap->heapArr);
	free(heap);
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heap_Insert( HEAP *heap, void *dataPtr)
{	
	(heap->last)++;
	
	if((heap->last) >= heap->capacity){
		(heap->capacity) *= 2;
		void** arr;
		heap->heapArr = (void**)realloc(heap->heapArr, sizeof(void*) * (heap->capacity));
	}
	
	(heap->heapArr)[heap->last] = dataPtr;
	
	_reheapUp(heap, heap->last);
	return 1;
}

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heap_Delete( HEAP *heap, void **dataOutPtr)
{
	if(heap_Empty(heap)) return 0;
	
	*dataOutPtr = heap->heapArr[0];
	heap->heapArr[0] = (heap->heapArr)[heap->last];
	(heap->last)--;
	
	_reheapDown(heap, 0);
	
	return 1;
}

/*
return 1 if the heap is empty; 0 if not
*/
int heap_Empty(  HEAP *heap)
{
	if(heap->last == -1) return 1;
	return 0;
}

/* Print heap array */
void heap_Print( HEAP *heap, void (*print_func) (void *data)){
	for(int i = 0; i <= heap->last; i++)
	{
		print_func((heap->heapArr)[i]);
	}
	printf("\n");
}

