#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

char* trim(char *input);

FILE* get_redirect_file(char *filepath);
void close_redirect_file(FILE *fp);

#endif