#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h>
#include <time.h> // time

#define RANDOM_INPUT	1
#define FILE_INPUT		2

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void);

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree);

/* internal function (not mandatory)
*/
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data);

/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr);

NODE *_makeNode( int data);

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree);
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);
/* internal traversal function
*/
static void _inorder_print( NODE *root, int level);

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int mode; // input mode
	TREE *tree;
	int data;
	
	if (argc != 2)
	{
		fprintf( stderr, "usage: %s FILE or %s number\n", argv[0], argv[0]);
		return 1;
	}
	
	FILE *fp;
	
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		mode = RANDOM_INPUT;
	}
	else mode = FILE_INPUT;
	
	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create a tree!\n");
		return 100;
	}

	if (mode == RANDOM_INPUT)
	{
		int numbers;
		numbers = atoi(argv[1]);
		assert( numbers > 0);

		fprintf( stdout, "Inserting: ");
		
		srand( time(NULL));
		for (int i = 0; i < numbers; i++)
		{
			data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
			
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
	}
	else if (mode == FILE_INPUT)
	{
		fprintf( stdout, "Inserting: ");
		
		while (fscanf( fp, "%d", &data) != EOF)
		{
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
		fclose( fp);
	}
	
	fprintf( stdout, "\n");

	if (BST_Empty( tree))
	{
		fprintf( stdout, "Empty tree!\n");
		BST_Destroy( tree);
		return 0;
	}	

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left inorder traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		if (scanf( "%d", &num) == EOF) break;
		
		int ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left inorder traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}










/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void){
	TREE* newTree;
	newTree = (TREE*)malloc(sizeof(TREE));
	newTree->root = NULL;
	
	return newTree;
}

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree){
	if(!BST_Empty(pTree));
		_destroy(pTree->root);
	free(pTree);
}

/* internal function (not mandatory)
*/
static void _destroy( NODE *root){
	if(root != NULL){
		_destroy(root->right);
		_destroy(root->left);
		free(root);
	}
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data){
	
	NODE* parent = NULL;
	int end = 0;
	NODE* newNode = NULL;
	newNode = _makeNode(data);
	if(pTree->root == NULL){
		pTree->root = newNode;
		return 1;
	}
	
	parent = pTree->root;
	
	while(1){
		
		if(parent->data > data){
			if(parent->left == NULL){
				parent->left = newNode;
				end = 1;
				break;
			}
			parent = parent->left;
		}
		
		else{
			if(parent->right == NULL){
				parent->right = newNode;
				end = 1;
				break;
			}
			parent = parent->right;
		}
	}
	
	return end;
}

/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr){
	 
}

NODE *_makeNode( int data){
	NODE* newNode = NULL;
	newNode = (NODE*)malloc(sizeof(NODE));
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey){
	int success;
	success = 0;
	pTree->root = _delete( pTree->root, dltKey, &success);
	return success;
}

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success){
	NODE* pvroot;
	pvroot = (NODE*)malloc(sizeof(NODE));
	pvroot->left = NULL;
	pvroot->right = NULL;	
	NODE* pNode = pvroot;
	NODE* cNode = root;
	NODE* dNode;
	
	pvroot->right = root;
	
	while(cNode != NULL && cNode->data != dltKey){
		pNode = cNode;
		
		if(dltKey < cNode->data){
			cNode = cNode->left;
		}
		else{
			cNode = cNode->right;
		}
	}
	
	if(cNode == NULL){
		*success = 0;
		free(pvroot);
		return root;
	}
	
	dNode = cNode;
	
	//1.삭제 노드 자식 없는 경우
	if(dNode->left == NULL && dNode->right == NULL){
		
		if(pNode->left == dNode){
			NODE* delNode;
			
			if(pNode != NULL){
				delNode = pNode->left;
				pNode->left = NULL;
			}
		}
		
		else{
			NODE* delNode;
			
			if(pNode != NULL){
				delNode = pNode->right;
				pNode->right = NULL;
			}
		} 
		
		if(dNode == root)
			root = pvroot->right;
	}
	
	//2.삭제 노드 자식이 하나인 경우
	else if(dNode->left == NULL || dNode->right == NULL){
		NODE* dcNode; // 삭제 대상의 자식 노드
		
		if(dNode->left != NULL)
			dcNode = dNode->left;
		else
			dcNode = dNode->right;
		
		if(pNode->left == dNode)
			pNode->left = dcNode;
		else
			pNode->right = dcNode;
		
		if(dNode == root)
			root = pvroot->right;
	}
	
	//3.삭제 노드 자식이 둘인 경우
	else{
		NODE* mNode = dNode->right; //대체 노드
		NODE* mpNode = dNode; //대체 노드의 부모노드
		int delData;
		
		while(mNode->left != NULL){
			mpNode = mNode;
			mNode = mNode->left;
		}
		
		delData = dNode->data;
		dNode->data = mNode->data;
		
		//대체 노드 부모노드와 대체 노드 자식노드 연결
		if(mpNode->right == mNode)
			mpNode->right = mNode->right;
		else
			mpNode->left = mNode->right;
		
		//삭제 노드가 루트노드
		if(dNode == root)
			root = pvroot->right;
		
		dNode = mNode;
		dNode->data = delData;
	}
	
	free(pvroot);
	free(dNode);
	*success = 1;
	return root;	
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key){
	NODE* pNode;
	pNode = _retrieve(pTree->root, key);
	
	if(pNode != NULL){
		return &(pNode->data);
	}
	
	return NULL;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key){
	NODE* pNode;
	pNode = root;
	
	while(pNode != NULL){
		if(key < pNode->data){
			pNode = pNode->left;
		}
		
		else if(key > pNode->data){
			pNode = pNode->right;
		}
		
		else{
			return pNode;
		}
	}
	
	return NULL;
}

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree){
	_traverse(pTree->root);
}

static void _traverse( NODE *root){
	if(root->left != NULL){
		_traverse(root->left);
	}
	printf("%d ", root->data);
	if(root->right != NULL){
		_traverse(root->right);
	}
}

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree){
	if(BST_Empty(pTree))
		return;
	_inorder_print(pTree->root, 0);
}
/* internal traversal function
*/
static void _inorder_print( NODE *root, int level){
	
	if(root->right != NULL){
		_inorder_print(root->right, level+1);
	}
	
	for(int i = 0; i < level; i++){
		printf("\t");
	}	
	printf("%d\n", root->data);
	
	if(root->left != NULL){
		
		_inorder_print(root->left, level+1);
	}
}

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree){
	if(pTree->root == NULL)
		return 1;
	return 0;
}