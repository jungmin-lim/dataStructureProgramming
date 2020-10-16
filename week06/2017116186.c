// 2017116186 jungmin lim
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 1024
#define MAX_EXPR_SIZE 1024

// precedence types
typedef enum {
    lparen, rparen, plus, minus, times, divide, eos, operand
}precedence;

// defining stucts
typedef struct treenode *treePointer;
typedef struct stacknode *stackPointer;
typedef struct queuenode *queuePointer;
typedef struct treenode{
    char data;
    treePointer leftChild, rightChild;
}treenode;
typedef struct stacknode{
    treePointer node;
    stackPointer link;
}stacknode;
typedef struct queuenode{
    treePointer node;
    int level;
    queuePointer link;
}queuenode;

precedence *stack;
char expr[MAX_EXPR_SIZE];
int top = -1;
int stack_size = 1024;
static int isp[] = {0, 19, 12, 12, 13, 13, 0};
static int icp[] = {20, 19, 12, 12, 13, 13, 0};

// infix to postfix
void stackEmpty(); // check if stack is empty. if empty exit
void stackFull(); // check if stack is full. if full, realloc
void push(int item); // push item in stack
precedence pop(void); // pop item from stack
precedence getToken(char *symbol, int *n); // compare char to precedence and return enum type
void postfix(FILE *f); // change infinx expr into postfix expr
void printToken(precedence token, FILE *f); // print precedence

// postfix to tree
void pushT(stackPointer *head, treePointer node); // push treenode into stack
treePointer popT(stackPointer *head); // pop treenode from stack 
int isOperand(char dat); // check if dat is operand
treePointer makeTree(); // make tree from postfix expr

// print tree data
void enqueue(queuePointer *queue, treePointer node, int level); // insert tree node and level info into queue
treePointer dequeue(queuePointer *queue); // delete front node from queue and return treenode from it
int getLevel(queuePointer queue); // get level from front of queue
void deleteTree(treePointer root); // delete everynode of tree
void levelOrder(treePointer node); // traverse tree levelorer and print every node

int main(int argc, char *argv[]){
    FILE * fp;
    char postinput[1024];
    int i = 0, j = 0;
    treePointer root = NULL;

    // exit if no argument
    if(argc != 2){
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        exit(-1);
    }

    // open input file read from argument
    if((fp = fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "Error while opening file %s\n", argv[1]);
        exit(-1);
    }

    // allocate memory for stack
    if((stack = (precedence *)malloc(sizeof(precedence) * stack_size)) == NULL){
        fprintf(stderr, "error while allocating memory\n");
        exit(-1);
    }

    // scan expr from input file
    while(fscanf(fp, "%c", &expr[i++]) != EOF){
        if(expr[i-1] == ' ') i--;
    }
    // close input file
    fclose(fp);

    // create & open postfix.txt file for saving postfix expr
    if((fp = fopen("postfix.txt", "w")) == NULL){
        fprintf(stderr, "Error while opening file %s\n", "postfix.txt");
        exit(-1);
    }

    // convert infix expr to postfix expr
    postfix(fp);
    // close postfix.txt
    fclose(fp);

    // open postfix.txt to to read postfix expr
    if((fp = fopen("postfix.txt", "r")) == NULL){
        fprintf(stderr, "Error while opening file %s\n", "postfix.txt");
        exit(-1);
    }

    // scan postfix expr from file
    fscanf(fp, "%s", postinput);
    // create tree
    root = makeTree();

    // traverse and print tree nodes
    levelOrder(root);

    // delete tree
    deleteTree(root);
    root = NULL;

    // close postfix.txt
    fclose(fp);
    return 0;
}

void stackEmpty(void){
    fprintf(stderr, "stack empty\n");
    exit(EXIT_FAILURE);
}

void stackFull(void){
    if((stack = (precedence *)realloc(stack, sizeof(precedence) * stack_size * 2)) == NULL){
        fprintf(stderr, "error while reallocating memory\n");
        exit(-1);
    }
    stack_size = stack_size * 2;
}

void push(int item){
    if(top >= stack_size){
        stackFull();
    }

    stack[++top] = item;
}

precedence pop(void){
    if(top == -1){
        stackEmpty();
    }
    return stack[top--];
}

precedence getToken(char *symbol, int *n){
    *symbol = expr[(*n)++];
    switch(*symbol){
        case '(':   return lparen;
        case ')':   return rparen;
        case '+':   return plus;
        case '-':   return minus;
        case '/':   return divide;
        case '*':   return times;
        case '\0':  return eos;
        default:    return operand;
    }
}

void postfix(FILE *f) {
    char symbol;
    precedence token;
    int n = 0;
    top = 0;
    stack[0] = eos;
    for (token = getToken(&symbol, &n); token != eos; token = getToken(&symbol, &n)) {
        if (token == operand) {
            fprintf(f, "%c", symbol);
        }

        else if(token == rparen){
            while (stack[top] != lparen) {
                printToken(pop(), f);
            }
            pop();
        }

        else{
            while (isp[stack[top]] >= icp[token]) {
                printToken(pop(), f);
            }
            push(token);
        }
    }

    while ((token = pop()) != eos) {
        printToken(token, f);
    }
}

void printToken(precedence token, FILE *f) {
    switch (token) {
        case plus:      fprintf(f, "+");
            break;
        case minus:     fprintf(f, "-");
            break;
        case divide:    fprintf(f, "/");
            break;
        case times:     fprintf(f, "*");
            break;
    }
}

void pushT(stackPointer *head, treePointer node) {
	stackPointer newStacknode;
	if((newStacknode = (stackPointer)malloc(sizeof(*newStacknode))) == NULL){
		fprintf(stderr, "Insufficient memory");
		exit(EXIT_FAILURE);
	}
	newStacknode->node = node;
	newStacknode->link = (*head);

	(*head) = newStacknode;
	return;
}

treePointer popT(stackPointer *head) {
	treePointer ret;
	stackPointer temp;
	if (!(*head)) {
		return NULL;
	}

	temp = (*head);
	(*head) = (*head)->link;

	ret = temp->node;
	free(temp);

	return ret;
}

void enqueue(queuePointer *queue, treePointer node, int level) {
	queuePointer newQueuenode, p = (*queue);
	if ((newQueuenode = (queuePointer)malloc(sizeof(*newQueuenode))) == NULL) {
		fprintf(stderr, "Insufficient memory");
		exit(EXIT_FAILURE);
	}
	newQueuenode->node = node;
    newQueuenode->level = level;
	if (!(*queue)) {
		newQueuenode->link = newQueuenode;
		(*queue) = newQueuenode;
		return;
	}

	while(p->link != (*queue)){
		p = p->link;
	}

	p->link = newQueuenode;
	newQueuenode->link = (*queue);
	return;
}

treePointer dequeue(queuePointer *queue) {
	queuePointer p, q;
	treePointer ret;
	if (!(*queue)) {
		return NULL;
	}

	p = (*queue);
	q = (*queue);
	while (q->link != (*queue)) {
		q = q->link;
	}

	(*queue) = (*queue)->link;
	if (p == (*queue)) {
		(*queue) = NULL;
	}
	else {
		q->link = (*queue);
	}

	ret = p->node;
	free(p);
	return ret;
}

int isOperand(char dat) {
	if (dat >= 'a' && dat <= 'z') {
		return 1;
	}
	return 0;
}

int getLevel(queuePointer queue){
    if(queue == NULL){
        return -1;
    }
    return queue->level;
}

treePointer makeTree() {
	FILE *f;
	treePointer newTreenode = NULL;
	stackPointer stack = NULL;
	char dat;
	
	f = fopen("postfix.txt", "r");
	while (fscanf(f, "%c", &dat) != EOF) {
		if (isOperand(dat)) {
			if ((newTreenode = (treePointer)malloc(sizeof(*newTreenode))) == NULL) {
				fprintf(stderr, "Insufficient memory");
				exit(EXIT_FAILURE);
			}
			newTreenode->data = dat;
			newTreenode->leftChild = NULL;
			newTreenode->rightChild = NULL;
			pushT(&stack, newTreenode);
		}
		else {
			if ((newTreenode = (treePointer)malloc(sizeof(*newTreenode))) == NULL) {
				fprintf(stderr, "Insufficient memory");
				exit(EXIT_FAILURE);
			}

			if (dat == '~') {
				newTreenode->data = dat;
				newTreenode->rightChild = popT(&stack);
				newTreenode->leftChild = NULL;
			}
			else {
				newTreenode->data = dat;
				newTreenode->rightChild = popT(&stack);
				newTreenode->leftChild = popT(&stack);
			}

			pushT(&stack, newTreenode);
		}
	}

	fclose(f);
	return newTreenode;
}

void deleteTree(treePointer root) {
	if (root) {
		deleteTree(root->leftChild);
		deleteTree(root->rightChild);
		free(root);
	}
}

void levelOrder(treePointer node) {
    int level = 1, front;
	queuePointer queue = NULL;
	if (!node) {
		return;
	}

	enqueue(&queue, node, 1);
    fprintf(stdout, "(Root, %c)\n", node->data);
	while(queue != NULL) {
        level = getLevel(queue);
		node = dequeue(&queue);
		if (node == NULL) {
            continue;
        }

        if (node->leftChild) {
			enqueue(&queue, node->leftChild, level+1);
            fprintf(stdout, "(%c, L, %c) ", node->data, node->leftChild->data);
        }

        if (node->rightChild) {
            enqueue(&queue, node->rightChild, level +1);
            fprintf(stdout, "(%c, R, %c) ", node->data, node->rightChild->data);
        }

        if(level != getLevel(queue)){
            fprintf(stdout, "\n");
        }
    }
	return;
}
