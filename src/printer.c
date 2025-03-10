#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "printer.h"

void print() {
  if (output == NULL) return;

  printf("%s\n", output);
}