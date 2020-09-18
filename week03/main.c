#include <stdio.h>
#include <stdlib.h>

int stackEmpty(int *);
int stackTop(int *);
int stackPop(int *); 
int stackPush(int *, int);

int size;
int top = -1;

// main
int main() {
	FILE *file;
	char filename[100];
	int output_top, i;
	int *input, *output;
	int *stack;

	// input filename
	printf("input file name: ");
	scanf("%s", filename);

	// open file
	if((file = fopen(filename, "r")) == NULL){
		fprintf(stderr, "error while reading %s\n", filename);
		exit(EXIT_FAILURE);
	}

	// read size
	fscanf(file, "%d", &size);

	// check input size
	if(size <= 0) {
		fprintf(stderr, "size must be larger than 0\n");
		exit(EXIT_FAILURE);
	}

	// allocate memory
	if((input = (int*)malloc(sizeof(int) * size)) == NULL){
		fprintf(stderr, "error while allocating memory\n");
		exit(EXIT_FAILURE);
	}
	if((output = (int*)malloc(sizeof(int) * size)) == NULL){
		fprintf(stderr, "error while allocating memory\n");
		exit(EXIT_FAILURE);
	}
	if((stack = (int*)malloc(sizeof(int) * size)) == NULL){
		fprintf(stderr, "error while allocating memory\n");
		exit(EXIT_FAILURE);
	}

	// reading input and output from file
	for(int i = 0; i < size; i++) {
		fscanf(file, "%d", &input[i]);
	}
	for(int i = 0; i < size; i++) {
		fscanf(file, "%d", &output[i]);
	}

	//solve
	output_top = 0;
	for(int i = 0; i < size; i++) {
		// while stack has data equals to output
		while(output[output_top] == stackTop(stack)) {
			output_top++;
			stackPop(stack);
		}

		// if input has data equals to output	
		if(input[i] == output[output_top]) {
			output_top++;
		}

		// if both input and output does not have data equls to output
		else{
			stackPush(stack, input[i]);
		}
	}

	// move stack data to output
	while (output[output_top] == stackTop(stack)) {
		output_top++;
		stackPop(stack);
	}

	// print result
	printf("input: ");
	for(i = 0; i < size; ++i){
		printf("%d ", input[i]);
	}
	printf("output: ");
	for(i = 0; i < size; ++i){
		printf("%d ", output[i]);
	}

	if (stackEmpty(stack)){
		printf("--> Yes\n");
	}
	else {
		printf("--> No\n");
	}

	// free array
	free(input);
	free(output);
	free(stack);

	// close file
	fclose(file);
	return 0;
}

int stackEmpty(int *stack){
	if(top == -1) return 1;
	return 0;
}

int stackTop(int *stack){
	if(stackEmpty(stack)) return -1;
	else return stack[top];
}

int stackPop(int *stack){
	if(stackEmpty(stack)) return -1;
	else return stack[top--];
}

int stackPush(int *stack, int data){
	if(top >= size) return -1;
	else stack[++top] = data;
	return 1;
}
