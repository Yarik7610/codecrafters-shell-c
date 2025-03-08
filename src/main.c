#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  printf("$ ");

  char input[100];
  fgets(input, 100, stdin);

  return 0;
}
