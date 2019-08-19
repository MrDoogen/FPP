#ifndef IO_BINARY_STREAMS_H
#define IO_BINARY_STREAMS_H
#endif

size_t file_read (char *file_name, char **content);
size_t file_write (char *file_name, char *content, size_t size_of_file);
size_t size_of_file (char *file_name);
