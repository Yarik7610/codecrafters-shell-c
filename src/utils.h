#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

char* trim(char *input);

FILE* get_redirect_file(char *file_path, char *file_mode);
void update_redirect_file_mode(char *file_mode, char *new_mode);
void close_redirect_file(FILE *fp);

#endif