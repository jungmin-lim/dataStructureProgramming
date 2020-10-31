//
//  main.c
//  DataStructureProgramming
//
//  Created by jungmin lim on 2020/10/30.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SWAP(x, y, t) ((t)=(x), (x)=(y), (y)=(t))

typedef struct {
    int key;
}element;

typedef struct leftist* leftistTree;
struct leftist {
    leftistTree leftChild;
    element data;
    leftistTree rightChild;
    int shortest;
};

typedef struct _queuenode* queuePointer;
struct _queuenode {
    leftistTree data;
    int level;
    struct _queuenode* next;
};

int treecount = 0, tsize = 0, front = 0, end = 0;
int treesize[1024];
char filename[1024];

// functions for lefistTree
int calcShortest(leftistTree); // return length of shortest path to external node
void minMeld(leftistTree* a, leftistTree* b); // meld of tree a, b
void minUnion(leftistTree* a, leftistTree* b); // meld of non-empty tree a, b

// functions for queue
void enqueue(queuePointer* queue, leftistTree node, int level); // insert tree node and level info into queue
leftistTree dequeue(queuePointer* queue); // delete front node from queue and return treenode from it
int getLevel(queuePointer queue); // get level from front of queue
void levelOrder(leftistTree node); // traverse tree levelorer and print every node

int main(int argc, const char* argv[]) {
    FILE* fd;
    leftistTree t1 = NULL, t2 = NULL;
    queuePointer queue = NULL;
    int input, inputcount = 0;

    // validate argument
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <INPUTFILE> <OUTPUTFILE>\n", argv[0]);
        exit(-1);
    }

    strcpy(filename, argv[2]);
    // open file
    if ((fd = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error while opening file %s\n", argv[1]);
        exit(-1);
    }

    // read keys from file
    while (fscanf(fd, "%d", &input) != EOF) {
        ++inputcount;
        // create new tree
        t2 = (leftistTree)malloc(sizeof(*t2));
        t2->data.key = input;
        t2->leftChild = NULL; t2->rightChild = NULL;
        t2->shortest = 1;

        // meld existing tree with new tree
        minMeld(&t1, &t2);

        // TODO : print tree, enqueue tree and make new tree (every other three inputs)
        if (inputcount == 4) {
            treesize[end++] = 4;
            tsize = 4;
            levelOrder(t1);
            enqueue(&queue, t1, 0);
            t1 = NULL;
            inputcount = 0;
        }
    }

    treesize[end++] = inputcount;
    tsize = inputcount;
    levelOrder(t1);
    enqueue(&queue, t1, 0);
    t1 = NULL;

    // TODO : make min leftist tree until queue is empty
    while (queue->next != NULL) {
        t1 = dequeue(&queue); tsize = treesize[front++];
        t2 = dequeue(&queue); tsize = tsize + treesize[front++];

        minMeld(&t1, &t2);
        levelOrder(t1);
        enqueue(&queue, t1, 0);
        treesize[end++] = tsize;
    }
    // close file
    fclose(fd);
    return 0;
}

int calcShortest(leftistTree x) {
    // if x is external node
    if (!x) {
        return 0;
    }

    return 1 + MIN(calcShortest(x->leftChild), calcShortest(x->rightChild));
}

void minMeld(leftistTree* a, leftistTree* b) {
    if (!*a) *a = *b;
    else if (*b) minUnion(a, b);
    *b = NULL;
}

void minUnion(leftistTree* a, leftistTree* b) {
    leftistTree temp = NULL;

    // if priority of a is larger than that of b
    if ((*a)->data.key > (*b)->data.key) SWAP(*a, *b, temp);

    // if right child of a is empty
    if (!(*a)->rightChild) (*a)->rightChild = (*b);
    // if right child of a is non-empty
    else minUnion(&(*a)->rightChild, b);

    if (!(*a)->leftChild) {
        (*a)->leftChild = (*a)->rightChild;
        (*a)->rightChild = NULL;
    }
    else if ((*a)->leftChild->shortest < (*a)->rightChild->shortest)
        SWAP((*a)->leftChild, (*a)->rightChild, temp);

    (*a)->shortest = (!(*a)->rightChild) ? 1 : (*a)->rightChild->shortest + 1;
}

void enqueue(queuePointer* queue, leftistTree node, int level) {
    queuePointer newQueuenode;
    queuePointer p = (*queue);
    if ((newQueuenode = (queuePointer)malloc(sizeof(*newQueuenode))) == NULL) {
        fprintf(stderr, "Insufficient memory");
        exit(EXIT_FAILURE);
    }
    newQueuenode->data = node;
    newQueuenode->level = level;
    if (!(*queue)) {
        newQueuenode->next = NULL;
        (*queue) = newQueuenode;
        return;
    }

    while (p->next != NULL) {
        p = p->next;
    }

    p->next = newQueuenode;
    newQueuenode->next = NULL;
    return;
}

leftistTree dequeue(queuePointer* queue) {
    queuePointer p, q;
    leftistTree ret;
    if (!(*queue)) {
        return NULL;
    }

    p = (*queue);
    q = (*queue);
    while (q->next != NULL) {
        q = q->next;
    }

    (*queue) = (*queue)->next;

    ret = p->data;
    free(p);
    return ret;
}

int getLevel(queuePointer queue) {
    if (queue == NULL) {
        return -1;
    }
    return queue->level;
}

void levelOrder(leftistTree node) {
    FILE* fd;
    int level = 1, checksize = 0;
    queuePointer queue = NULL;

    // open file
    if ((fd = fopen(filename, "a")) == NULL) {
        fprintf(stderr, "Error while opening file %s\n", filename);
        exit(-1);
    }

    if (!node) {
        return;
    }
    fprintf(fd, "%dth tree\n", ++treecount);
    enqueue(&queue, node, 1);

    while (queue != NULL) {
        level = getLevel(queue);
        node = dequeue(&queue);
        if (node == NULL) {
            fprintf(fd, "%% ");
            enqueue(&queue, NULL, level + 1);
            enqueue(&queue, NULL, level + 1);
        }

        else {
            enqueue(&queue, node->leftChild, level + 1);
            enqueue(&queue, node->rightChild, level + 1);

            fprintf(fd, "%d ", node->data.key);
            checksize++;

            if (checksize == tsize) break;

        }
        if (level != getLevel(queue)) {
            fprintf(fd, "\n");
        }
    }

    fprintf(fd, "\n");
    fclose(fd);
    return;
}

