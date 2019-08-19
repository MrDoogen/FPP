#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "proto.h"

size_t file_read (char *file_name, char **content) {
	FILE *file = fopen(file_name, "rb");

	if (file == NULL) {
		return ERROR_FILE;
	}

	fseek(file, 0, SEEK_END);
	size_t size_of_file = ftell(file);
	rewind(file);

	*content = (char*) malloc((size_of_file + 1) * sizeof(char));

	if (*content == NULL) {
		return ERROR_MEMORY;
	}

	size_t bytes_read = fread(*content, sizeof(char), size_of_file, file);

	if (size_of_file != bytes_read) {
		return ERROR_FILE;
	}

	fclose(file);
	(*content)[size_of_file] = '\0';
	
	return size_of_file;
}


size_t file_write (char *file_name, char *content, size_t size_of_file) {
	FILE *file = fopen(file_name, "wb");

	if (file == NULL) {
		return ERROR_FILE;
	}

	fwrite(content, size_of_file, 1, file);
	fclose(file);
	return 0;
}


size_t size_of_file (char *file_name) {
	FILE *file = fopen(file_name, "rb");
	if (file == NULL) {
		return ERROR_FILE;
	}

	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	rewind(file);
	
	return file_size;
}
