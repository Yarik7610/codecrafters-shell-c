#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

char* ltrim(char *input) {
  if (input == NULL || *input == '\0') return input;

  while (isspace(*input)) ++input;

  return input;
}

char* rtrim(char *input) {
  if (input == NULL || *input == '\0') return input;

  char *end = input + strlen(input);
  while (end >= input && isspace(*(end - 1))) --end;

  *end = '\0';

  return input;
}

char* trim(char *input) {
  if (input == NULL || *input == '\0') return input;

  return rtrim(ltrim(input));
}

FILE* get_redirect_file(char *filepath) {
  FILE* redirect_file = NULL; 

  if (filepath) {
    redirect_file = fopen(filepath, "w");
    if (!redirect_file) {
      perror("Can't open output file");
      exit(1);
    }
  }

  return redirect_file;
}

void close_redirect_file(FILE *fp) {
  if (fp != stdout && fp != stderr) fclose(fp);
}

