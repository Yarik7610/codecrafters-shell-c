#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "main.h"

char* trim(char *input);

FILE* get_redirect_file(RedirectFileInfo *redirect_info);
RedirectFileInfo* get_redirect_file_info(char file_descriptor); 
void update_redirect_file_mode(RedirectFileInfo *redirect_info, char *new_mode);
void close_redirect_file(FILE *fp);

#endif