#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 100
#define EXIT_COMMAND "exit 0"

int main() {
  setbuf(stdout, NULL);
  
  char input[MAX_INPUT_SIZE];
  int input_len = 1;

  while (1) {
    printf("$ ");

    fgets(input, MAX_INPUT_SIZE, stdin);

    input_len = strlen(input);
    input[input_len - 1] = '\0';

    if (strcmp(input, EXIT_COMMAND) == 0) exit(0);

    printf("%s: command not found\n", input);
  }

  return 0;
}
