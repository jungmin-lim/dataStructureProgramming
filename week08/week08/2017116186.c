// 2017116186 jungmin
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct bucket {
	char key[2][2];
	int dataCount;
}BUCKET;

// initialize size values
int MASK_SIZE = 1;
int DIR_SIZE = 2;
int SLOT_SIZE = 2;

int hash(char*); // hash function
int main(int argc, char* argv[]) {
	FILE* fp;
	BUCKET** hashTable;
	BUCKET* newBucket;
	char buffer[1024], temp;
	int i = 0, j = 0, k = 0;
	int hashedValue, maskedValue;

	// check usage
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <FILE NAME>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// open file
	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Error while opening %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	// create hash table
	if ((hashTable = malloc(sizeof(BUCKET*) * DIR_SIZE)) == NULL) {
		fprintf(stderr, "Error while allocating memory\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < DIR_SIZE; ++i) {
		hashTable[i] = NULL;
	}
	
	// scan input from file
	i = 0;
	while ((fscanf(fp, "%c", &temp)) != EOF) {
		// insert buffer
		if (temp >= 'A' && temp <= 'H') {
			buffer[i++] = temp;
		}

		// insert hash
		if (i == 2) {
			hashedValue = hash(buffer);
			maskedValue = (hashedValue & ((1 << MASK_SIZE) - 1));
			// if bucket is empty
			if (hashTable[maskedValue] == NULL) {
				// create new bucket
				if ((newBucket = malloc(sizeof(BUCKET))) == NULL) {
					fprintf(stderr, "Error while allocating memory\n");
					exit(EXIT_FAILURE);
				}
				newBucket->dataCount = 1;
				newBucket->key[0][0] = buffer[0];
				newBucket->key[0][1] = buffer[1];

				// assign pointer to bucket for hashTable
				hashTable[maskedValue] = newBucket;
			}
			// if bucket is not full
			else if(hashTable[maskedValue]->dataCount < 2) {
				hashTable[maskedValue]->dataCount = 2;
				hashTable[maskedValue]->key[1][0] = buffer[0];
				hashTable[maskedValue]->key[1][1] = buffer[1];
			}
			// if bucket is full
			else {

			}

			// print hashTable
			printf("Directory(%d): ", DIR_SIZE);
			for (j = 0; j < DIR_SIZE; ++j) {
				if (hashTable[j] == NULL) printf("null ");
				else printf("%p ", hashTable[j]);
			}
			printf("\n");

			for (j = 0; j < DIR_SIZE; ++j) {
				if (!(hashTable[j] == NULL)) {
					printf("%p: ", hashTable[j]);
					for (k = 0; k < hashTable[j]->dataCount; ++k) {
						printf("%c%c ", hashTable[j]->key[k][0], hashTable[j]->key[k][1]);
					}
				}
			}
			printf("\n");
			i = 0;
		}
	}

	// close file
	fclose(fp);
	return 0;
}

int hash(char* input) {
	return ((input[0] - 65) * 8 + (input[1] - 65));
}