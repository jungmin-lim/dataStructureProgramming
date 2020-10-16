// 2017116186 jungmin

// compiled with
// gcc --version
// Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/4.2.1
// Apple clang version 12.0.0 (clang-1200.0.32.2)
// Target: x86_64-apple-darwin20.1.0 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTEX 1024
#define TRUE 1
#define FALSE 0

short int visited[MAX_VERTEX];
short int parent[MAX_VERTEX];
short int childCycle, parentCycle;
short int isCycle = FALSE;

typedef struct linkedNode *linkedPointer;
typedef struct linkedNode {
	int data;
	linkedPointer next;
}linkedNode;

void addFront(linkedPointer *head, int data);
void printList(linkedPointer head);
void dfs(int vertex, linkedPointer adList[]);
void isCyclic(int vertex, linkedPointer adList[]);
void solve(int vertexCount, linkedPointer adList[]);

int main(int argc, char *argv[]) {
	FILE *f;
	linkedPointer adList[MAX_VERTEX] = { NULL };
	int vertexCount, edgeCount;
	int a, b, i, j;

    if(argc != 2){
        fprintf(stderr, "Usage: %s <INPUT FILENAME>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

	if((f = fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "Error while opening file\n");
        exit(EXIT_FAILURE);
    }

	fscanf(f, "%d %d", &vertexCount, &edgeCount);
	
	for (i = 0; i < edgeCount; i++) {
		fscanf(f, "%d %d", &a, &b);

		addFront(&adList[a], b);
		addFront(&adList[b], a);
	}

    solve(vertexCount, adList);

	fclose(f);
	return 0;
}

void addFront(linkedPointer *head, int data) {
	linkedPointer newNode;
	if ((newNode = (linkedPointer)malloc(sizeof(*newNode))) == NULL) {
		fprintf(stderr, "Insufficient Memory");
		exit(EXIT_FAILURE);
	}
	newNode->data = data;
	newNode->next = *head;

	*head = newNode;
	return;
}

void printList(linkedPointer head) {
	while (head != NULL) {
		fprintf(stdout, "%5d", head->data);
		head = head->next;
	}
	return;
}

void solve(int vertexCount, linkedPointer adList[]){
    int i, j;

    // print components
    j = 0;
    for(i = 0; i < vertexCount+1; ++i){
        visited[i] = FALSE;
    }
    for(i = 1; i < vertexCount+1; ++i){
        if(visited[i]) continue;

        j++;
        fprintf(stdout, "connected component :");
        dfs(i, adList);
        fprintf(stdout, "\n");
    }

    // is it tree?
    if(j > 1){
        fprintf(stdout, "This is not a tree. Graph is not connected\n");
        return;
    }

    // cyclic test
    for(i = 0; i < vertexCount+1; ++i){
        visited[i] = FALSE;
    }
    for(i = 1; i < vertexCount+1; ++i){
        if(visited[i]) continue;

        isCyclic(i, adList);
        if(isCycle){
            fprintf(stdout, "cycle : (%d, %d)\n", parentCycle, childCycle);
            fprintf(stdout, "This is not a tree. Graph has a cycle");
            break;
        }
    }


}
void dfs(int vertex, linkedPointer adList[]) {
	linkedPointer w;
	visited[vertex] = TRUE;
	printf("%5d", vertex);
	for (w = adList[vertex]; w; w = w->next) {
		if (!visited[w->data]) {
			dfs(w->data, adList);
		}
	}
}

void isCyclic(int vertex, linkedPointer adList[]) {
    linkedPointer w;
    visited[vertex] = TRUE;

    for(w = adList[vertex]; w; w = w->next){
        if(!visited[w->data]){
            parent[w->data] = vertex;
            isCyclic(vertex, adList);
        }
        else if(parent[vertex] != w->data){
            childCycle = w->data;
            parentCycle = vertex;
            isCycle = TRUE;
            return;
        }
    }
}
