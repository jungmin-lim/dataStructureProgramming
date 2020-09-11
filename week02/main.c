// 2017116186 임정민

#include <stdio.h>
#include <stdlib.h>

// main
int main(int argc, char *argv[]){
	char filename[30];
	int input, i = 0, max = 0;
	int *count, *number;
	FILE *file;

	// get filename from user input
	printf("input filename: ");
	scanf("%s", filename);

	// read file
	if((file = fopen(filename, "r")) == NULL){
		fprintf(stderr, "error while opening file");
		exit(EXIT_FAILURE);
	}

	// read size of input data
	fscanf(file, "%d", &input);

	// allocating memory for data
	if ((count = (int *)malloc(input * sizeof(int))) == NULL){
		fprintf(stderr, "Error while allocating memory");
		exit(EXIT_FAILURE);
	}
	if ((number = (int *)malloc(input * sizeof(int))) == NULL){
		fprintf(stderr, "Error while allocating memory");
		exit(EXIT_FAILURE);
	}

	// initialize array
	for(i = 0; i < input; ++i){
		number[i] = 0;
		count[i] = 0;
	}

	// reading every number
	while(fscanf(file, "%d", &input) != EOF){
		// search if same number exist
		for(i = 0; i < max; ++i){
			if(number[i] == input){
				count[i]++;
				break;
			}
		}

		// if same number does not exist
		if(i >= max){
			max++;
			number[i] = input;
			count[i]++;
		}
	}

	// print result
	for(i = 0; i < max; ++i){
		printf("number %d showed up %d time(s) \n", number[i], count[i]);
	}

	// free memory allocation
	free(number);
	free(count);

	// close file
	fclose(file);

	return 0;
}
