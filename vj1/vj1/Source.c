#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define ERROR_OF -1
#define BUFFER_SIZE 50;

int count_rows(char* filename);


int main() {
	
	return 0;
}


int count_rows(char* filename) {
	FILE* fp = NULL;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error opening file!\n");
		return ERROR_OF;
	}

	while (!feof(fp)) {
	}
	return 0;
}