#include <string.h>
#include <ctype.h>
#include "trim.h"

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