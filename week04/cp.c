// 2017116186 ���
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct treenode{
	int data;
	struct treenode *left;
	struct treenode *right;
} TREENODE;

typedef struct queuenode{
	TREENODE *data;
	int depth;
	struct queuenode *next;
} QUEUENODE;

TREENODE *makeTreeNode(int data); // create new tree node
void insertTree(TREENODE *tree, TREENODE *newNode); // insert new node in tree
TREENODE* makeTree(FILE* file); // create tree from input
QUEUENODE *makeQueueNode(TREENODE *data, int level); // create new queue node
void insertQueue(QUEUENODE **queue, TREENODE *data, int level); // insert new node in queue
int getFrontLevel(QUEUENDOE* queue); // get level of front node from queue
TREENODE *deleteQueue(QUEUENODE **queue); // delete from node from queue
void solve(QUEUENODE *queue, TREENODE *tree); // solve probelm
void freeTree(TREENODE **tree); // free allocated memory from tree

int main(int argc, char *argv[]) {
	FILE* file;
	char filename[100];
	TREENODE *tree = NULL;
	QUEUENODE *queue = NULL;

	// input filename from user
	printf("input filename: ");
	scanf("%s", filename);

 	// open file
	if((file = fopen(filename, "r")) == NULL){
		fprintf(stderr, "error while opening file\n");
		exit(EXIT_FAILURE);
	}

	// create tree from input file
	tree = makeTree(file);

	// insert root node in queue
	insertQueue(&queue, tree, 1);
	// solve problem
	solve(queue, tree);

	// free allocated memory
	freeTree(&tree);
	fclose(file);
	
	return 0;
}

TREENODE *makeTreeNode(int data) {
	TREENODE *newNode 
	
	// allocate memory
	if((newNode = (TREENODE *)malloc(sizeof(TREENODE))) == NULL){
		fprintf(stderr, "error while allocating memory\n");
		exit(EXIT_FAILURE);
	}

	// initialize memory
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

void insertTree(TREENODE *tree, TREENODE *newNode){
	// if insert data is samller
	if (tree->data > newNode->data){
		if (tree->left == NULL){
			tree->left = newNode;
		}
		else{
			insertTree(tree->left, newNode);
		}
	}
	// if insert data is larger
	else{
		if (tree->right == NULL){
			tree ->right = newNode;
		}
		else{
			insertTree(tree->right, newNode);
		}
	}
}

TREENODE* makeTree(FILE* file) {
	int input;
	TREENODE *root;
	TREENODE *inputTree;

	// create root node
	fscanf(file, "%d", &input);
	root = makeTreeNode(input);

	// create tree from input
	while (fscanf(file, "%d", &input) != EOF) {
		inputTree = makeTreeNode(input);
		insertTree(root, inputTree);
	}

	return root;
}

QUEUENODE *makeQueueNode(TREENODE *data, int level) {
	QUEUENODE *newNode;
	// allocate new node
	if((newNode = (QUEUENODE *)malloc(sizeof(q_node))) == NULL){
		fprintf(stderr, "error while allocating memory\n");
		exit(EXIT_FAILURE);
	}

	// initialize new node
	newNode->data = data;
	newNode->level = level;
	newNode->next = NULL;

	return newNode;
}

void insertQueue(QUEUENODE **queue, TREENODE *data, int level) {
	QUEUENODE *tempNode;
	
	// if queue empty
	if ((*queue) == NULL) {
		(*queue) = makeQueueNode(data, level);
		return;
	}

	tempNode = (*queue);
	// traverse to last node
	while (tempNode->next != NULL){
		tempNode = tempNode->next;
	}

	// insert new node at last
	tempNode->next = makeQueueNode(data, level);
}

int getFrontLevel(QUEUENDOE* queue){
	// if queue empty
	if (queue == NULL){
		fprintf(stderr, "queue empty\n");
		exit(EXIT_FAILURE);
	}

	return queue->level;
}

TREENODE *deleteQueue(QUEUENODE **queue){
	// if queue empty
	if ((*queue) == NULL) {
		fprintf(stderr, "queue empty\n");
		exit(EXIT_FAILURE);
	}

	TREENODE *data = (*queue)->data;
	QUEUENODE *tempNode = (*queue);

	// set queue to next node
	(*queue) = (*queue)->next;
	free(tempNode);
	return data;
}

// solve problem
void solve(QUEUENODE *queue, TREENODE *tree) {
	int front;
	int level = 1;
	TREENODE *data;

	while (queue != NULL) {
		// get data from queue front
		level = getFrontLevel(queue);
		data = deleteQueue(&queue);
		if (data == NULL){
			continue;
		}

		if (data->left != NULL){
			insertQueue(&queue, data->left, level + 1);
		}
		if (data->right != NULL){
			insertQueue(&queue, data->right, level + 1);
		}

		// if node is the most right node of tree
		if (level != getFrontLevel(queue)) {
			printf("(%d %d) ", level, data->data);
		}
	}
	
	printf("\n");
}

void freeTree(TREENODE **tree) {
	if ((*tree)->left != NULL){
		freeTree(&(*tree)->left);
	}
	if ((*root)->right != NULL){
		freeTree(&(*tree)->right);
	}

	free(*tree);
	*tree = NULL;
	return;
}
