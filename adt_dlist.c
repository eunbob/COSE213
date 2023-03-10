#include <stdlib.h> // malloc

#include "adt_dlist.h"

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, void *dataInPtr)
{
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	
	if(newNode == NULL) 
		return 0;

	newNode->dataPtr = dataInPtr;
	
	if( pList->count == 0){
		pList->head = newNode;
		pList->rear = newNode;
		newNode->rlink = newNode->llink = NULL;
		return 1;
	}
	
	//맨 앞에 추가
	if(pPre == NULL){
		newNode->llink = NULL;
		newNode->rlink = pList->head;
		pList->head->llink = newNode;
		pList->head = newNode;
		return 1;
	}
	
	//중간에 추가
	newNode->rlink = pPre->rlink;
	newNode->llink = pPre;
	if(pPre->rlink != NULL)
		pPre->rlink->llink = newNode;
	pPre->rlink = newNode;
	
	//맨뒤에 추가됨
	if(newNode->rlink == NULL) pList->rear = newNode;
	return 1;
}

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr)
{
	NODE* delNode = NULL;
	delNode = pLoc;
	
	pPre->rlink = pLoc->rlink;
	pLoc->rlink->llink = pPre;
	
	*dataOutPtr = delNode->dataPtr;
}

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu)
{
	NODE* cur = NULL;
	int cmp;
	
	cur = pList->head;
	
	cmp = pList->compare(cur->dataPtr, pArgu);
	
	*pPre = NULL;
	
	while(cmp <= 0){
		
		if(cmp == 0){
			*pLoc = cur;
			return 1;
		}
		
		*pPre = cur;
		cur = cur->rlink;
		if(cur == NULL) break;
		cmp = pList->compare(cur->dataPtr, pArgu);
	}
	
	return 0;
}


/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList( int (*compare)(const void *, const void *))
{
	LIST *list = (LIST*)malloc(sizeof(LIST));
	
	if(list == NULL) return NULL;
	
	list->count = 0;
	list->head = NULL;
	list->rear = NULL;
	list->compare = compare;
	
	return list;
}

/* Deletes all data in list and recycles memory
*/
void destroyList( LIST *pList, void (*callback)(void *))
{
	NODE *delNode;
	
	while( pList->head != NULL ){
		delNode = pList->head;
		pList->head = delNode->rlink;
		
		callback(delNode->dataPtr);
		free(delNode);
	}
	
	free(pList);
}

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode( LIST *pList, void *dataInPtr, void (*callback)(const void *, const void *))
{
	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	int check;
	
	if(pList->count == 0){
		_insert(pList, pPre, dataInPtr);
		pList->count += 1;
		return 1;
	}
	
	check = _search(pList, &pPre, &pLoc, dataInPtr);
	
	if(check){
		callback(pLoc->dataPtr, dataInPtr);
		return 2;
	}
	
	
	if(_insert(pList, pPre, dataInPtr) == 0) return 0;
	
	pList->count += 1;
	return 1;
}

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, void *keyPtr, void **dataOutPtr)
{
	NODE* pPre = NULL;
	NODE* pLoc = NULL;
	
	if(_search(pList, &pPre, &pLoc, keyPtr)){
		_delete( pList, pPre, pLoc, dataOutPtr);
		pList->count -= 1;
		
		free(pLoc);
		return 1;
	}
	return 0;
}

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, void *pArgu, void **dataOutPtr)
{
	NODE* pPre = NULL;
	NODE* pLoc = NULL;
	
	if(_search(pList, &pPre, &pLoc, pArgu)){
		*dataOutPtr = pLoc->dataPtr;
		return 1;
	}
	return 0;
}

/* returns number of nodes in list
*/
int countList( LIST *pList)
{
	return pList->count;
}

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList)
{
	if(pList->count)
		return 0;
	else
		return 1;
}

/* traverses data from list (forward)
*/
void traverseList( LIST *pList, void (*callback)(const void *))
{
	NODE* cur = NULL;
	cur = pList->head;
	
	while(cur != NULL){
		callback(cur->dataPtr);
		cur = cur->rlink;
	}
}

/* traverses data from list (backward)
*/
void traverseListR( LIST *pList, void (*callback)(const void *))
{
	NODE* cur = NULL;
	cur = pList->rear;
	
	while(cur != NULL){
		callback(cur->dataPtr);
		cur = cur->llink;
	}
}


