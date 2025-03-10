#include "printer.h"
#include <stdio.h>
#include <stdlib.h>

void print(char **output) {
  if (output == NULL) return;

  for (int i = 0; output[i] != NULL; ++i) {
    printf("%s ", output[i]);
  }

  printf("\n");
}