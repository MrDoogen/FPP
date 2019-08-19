#ifndef IO_TEXT_STREAMS_H
#define IO_TEXT_STREAMS_H
#endif

size_t text_read_line (char *file_name, char **content, size_t target_line);
size_t text_write (char *file_name, char *content, char mode);
