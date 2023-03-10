#include <stdlib.h> // malloc
#include <string.h> // strchr
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define SEARCH			4
#define DELETE			5
#define COUNT			6

// User structure type definition
typedef struct 
{
	char	*name;
	char	sex;
	int		freq;
} tName;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tName		*dataPtr;
	struct node	*llink;
	struct node	*rlink;
} NODE;

typedef struct
{
	int		count;
	NODE	*head;
	NODE	*rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList(void);

/* Deletes all data in list and recycles memory
*/
void destroyList( LIST *pList);

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode( LIST *pList, tName *dataInPtr);

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, tName *keyPtr, tName **dataOutPtr);

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, tName *pArgu, tName **dataOutPtr);

/* returns number of nodes in list
*/
int countList( LIST *pList);

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList);

/* traverses data from list (forward)
*/
void traverseList( LIST *pList, void (*callback)(const tName *));

/* traverses data from list (backward)
*/
void traverseListR( LIST *pList, void (*callback)(const tName *));

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr);

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tName **dataOutPtr);

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu);

////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a name structure, initialize fields(name, freq) and returns its address to caller
	return	name structure pointer
			NULL if overflow
*/
tName *createName( char *str, char sex, int freq); 

/* Deletes all data in name structure and recycles memory
*/
void destroyName( tName *pNode);

////////////////////////////////////////////////////////////////////////////////
/* gets user's input
*/
int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'P':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'S':
			return SEARCH;
		case 'D':
			return DELETE;
		case 'C':
			return COUNT;
	}
	return 0; // undefined action
}

// compares two names in name structures
// for createList function
int cmpName( const tName *pName1, const tName *pName2)
{
	int ret = strcmp( pName1->name, pName2->name);
	if (ret == 0) return pName1->sex - pName2->sex;
	else return ret;
}

// prints contents of name structure
// for traverseList and traverseListR functions
void print_name(const tName *dataPtr)
{
	printf( "%s\t%c\t%d\n", dataPtr->name, dataPtr->sex, dataPtr->freq);
}

// increases freq in name structure
// for addNode function
void increase_freq(tName *dataOutPtr, const tName *dataInPtr)
{
	dataOutPtr->freq += dataInPtr->freq;
}

// splits name and sex from string
// ex) "Zoe/M"
void split_name_sex(char *str, char *sex)
{
	char *p;
	
	p = strchr(str, '/');
	if (p == NULL) 
	{
		fprintf( stderr, "unexpected name/sex format\n");
		return;
	}
	else
	{
		*p++ = 0;
		*sex = *p;
	}
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	
	char str[1024];
	char name[100];
	char sex;
	int freq;
	
	tName *pName;
	int ret;
	FILE *fp;
	
	if (argc != 2){
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	// creates an empty list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	while(fscanf( fp, "%*d\t%s\t%c\t%d", str, &sex, &freq) != EOF)
	{
		pName = createName( str, sex, freq);
		
		ret = addNode( list, pName);
		
		if (ret == 2) // duplicated
		{
			destroyName( pName);
		}
	}
	
	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{
		tName *p;
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list);
				return 0;
			
			case FORWARD_PRINT:
				traverseList( list, print_name);
				break;
			
			case BACKWARD_PRINT:
				traverseListR( list, print_name);
				break;
			
			case SEARCH:
				fprintf( stderr, "Input a name/sex to find: ");
				fscanf( stdin, "%s", str);
				split_name_sex(str, &sex);
				
				pName = createName( str, sex, 0);

				if (searchList( list, pName, &p)) print_name( p);
				else fprintf( stdout, "%s not found\n", str);
				
				destroyName( pName);
				break;
				
			case DELETE:
				fprintf( stderr, "Input a name/sex to delete: ");
				fscanf( stdin, "%s", str);
				split_name_sex(str, &sex);
				
				pName = createName( str, sex, 0);

				if (removeNode( list, pName, &p))
				{
					fprintf( stdout, "(%s, %c, %d) deleted\n", p->name, p->sex, p->freq);
					destroyName( p);
				}
				else fprintf( stdout, "%s not found\n", str);
				
				destroyName( pName);
				break;
			
			case COUNT:
				fprintf( stdout, "%d\n", countList( list));
				break;
		}
		
		if (action) fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}

LIST *createList(void){
	LIST *list = (LIST*)malloc(sizeof(LIST));
	
	if(list == NULL) return NULL;
	
	list->count = 0;
	list->head = NULL;
	list->rear = NULL;
	
	return list;
}

void destroyList( LIST *pList){
	NODE *delNode;
	
	while( pList->head != NULL ){
		delNode = pList->head;
		pList->head = delNode->rlink;
		
		destroyName(delNode->dataPtr);
		free(delNode);
	}
	
	free(pList);
}

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode( LIST *pList, tName *dataInPtr){
	
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
		increase_freq(pLoc->dataPtr, dataInPtr);
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
int removeNode( LIST *pList, tName *keyPtr, tName **dataOutPtr){
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
int searchList( LIST *pList, tName *pArgu, tName **dataOutPtr){
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
int countList( LIST *pList){
	return pList->count;
}

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList){
	if(pList->count)
		return 0;
	else
		return 1;
}

/* traverses data from list (forward)
*/
void traverseList( LIST *pList, void (*callback)(const tName *)){
	
	NODE* cur = NULL;
	cur = pList->head;
	
	while(cur != NULL){
		callback(cur->dataPtr);
		cur = cur->rlink;
	}
}

/* traverses data from list (backward)
*/
void traverseListR( LIST *pList, void (*callback)(const tName *)){
	NODE* cur = NULL;
	cur = pList->rear;
	
	while(cur != NULL){
		callback(cur->dataPtr);
		cur = cur->llink;
	}
}

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr){
	
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
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tName **dataOutPtr){
	NODE* delNode = NULL;
	delNode = pLoc;
	
	pPre->rlink = pLoc->rlink;
	pLoc->rlink->llink = pPre;
	
	*dataOutPtr = delNode->dataPtr;
}

/* internal search function
	searches list and passes back address of node containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu){
	
	NODE* cur = NULL;
	int cmp;
	
	cur = pList->head;
	
	cmp = cmpName(cur->dataPtr, pArgu);
	
	*pPre = NULL;
	
	while(cmp <= 0){
		
		if(cmp == 0){
			*pLoc = cur;
			return 1;
		}
		
		*pPre = cur;
		cur = cur->rlink;
		if(cur == NULL) break;
		cmp = cmpName(cur->dataPtr, pArgu);
	}
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a name structure, initialize fields(name, freq) and returns its address to caller
	return	name structure pointer
			NULL if overflow
*/
tName *createName( char *str, char sex, int freq){
	tName* pName = (tName*)malloc(sizeof(tName));
	
	if(pName == NULL) return NULL;
	
	char *name;
	name = (char*)malloc( sizeof(char) * (strlen(str) + 1));
	strcpy(name, str);
	
	pName->name = name;
	pName->sex = sex;
	pName->freq = freq;
	
	return pName;
}

/* Deletes all data in name structure and recycles memory
*/
void destroyName( tName *pNode){
	free(pNode->name);
	free(pNode);
}
