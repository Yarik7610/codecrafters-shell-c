#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "parser.h"
#include "utils.h"
#include "evaluator.h"

char *flags = NULL;
int flags_count = 0;

char **arguments = NULL;
int arguments_count = 0;

RedirectFileInfo redirect_out_info = {NULL, "w"};
RedirectFileInfo redirect_err_info = {NULL, "w"};

void free_globals();

int main() {
  setbuf(stdout, NULL);
  
  char input[MAX_INPUT_SIZE];

  while (1) {
    printf("$ ");

    fgets(input, MAX_INPUT_SIZE, stdin);

    input[strlen(input) - 1] = '\0';

    char *trimmed_input = trim(input);
    if (*trimmed_input == '\0') continue;
   
    parse_input(trimmed_input);

    evaluate(trimmed_input);

    free_globals();
  }

  return 0;
}


void free_globals() {
  free(flags);
  flags = NULL;
  flags_count = 0;

  for (int i = 0; i < arguments_count; ++i) {
    free(arguments[i]);
    arguments[i] = NULL;
  } 
  free(arguments);
  arguments = NULL;
  arguments_count = 0;

  free(redirect_out_info.path);
  redirect_out_info.path = NULL;
  update_redirect_file_mode(&redirect_out_info, "w");

  free(redirect_err_info.path);
  redirect_err_info.path = NULL;
  update_redirect_file_mode(&redirect_err_info, "w");
}