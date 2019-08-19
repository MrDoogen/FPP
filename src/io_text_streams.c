#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "proto.h"

#define MEMPOOL_SIZE 1000


size_t text_read_line (char *file_name, char **content, size_t target_line) {
	size_t readed_size = 0;
	size_t multiplier = 1;
	size_t line_counter = 1;
	char symbol = '0';

	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		return ERROR_FILE;
	}

	*content = (char *) malloc(MEMPOOL_SIZE * sizeof(char));
	if (*content == NULL) {
		return ERROR_MEMORY;
	}

	for (size_t i = 0; (symbol = fgetc(file)) != EOF; i++) {
		if (line_counter == target_line) {
			(*content)[readed_size] = symbol;
			readed_size += 1;
			if (!(readed_size < MEMPOOL_SIZE * multiplier)) {
				multiplier += 1;
				*content = (char *) realloc(*content, (multiplier * MEMPOOL_SIZE) * sizeof(char));
				if (*content == NULL) {
					return ERROR_MEMORY;
				}
			}
		}

		if (line_counter > target_line) {
			break;
		}

		if (symbol == '\n') {
			line_counter += 1;
			i = 0;
		}
	}

	fclose(file);

	*content = (char *) realloc(*content, (readed_size + 1) * sizeof(char));
	if (*content == NULL) {
		return ERROR_MEMORY;
	}

	(*content)[readed_size] = '\0';
	return readed_size;
}


size_t text_write (char *file_name, char *content, char mode) {
	FILE *file = NULL;
	switch (mode) {
		case 'a':
			file = fopen(file_name, "a");
			break;
		case 'w':
			file = fopen(file_name, "w");
			break;
	}
	
	if (file == NULL) {
		return ERROR_FILE;
	}
	
	fputs(content, file);
	fclose(file);
	return 0;
}
