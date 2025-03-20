#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "main.h"
#include "parser.h"
#include "utils.h"
#include "evaluator.h"
#include "stdin.h"
#include "autocomplete.h"

char *flags = NULL;
int flags_count = 0;

char **arguments = NULL;
int arguments_count = 0;

RedirectFileInfo redirect_out_info = {NULL, "w"};
RedirectFileInfo redirect_err_info = {NULL, "w"};

void free_globals();

int main() {
  struct termios orig_termios;
  enable_raw_mode(&orig_termios);

  setbuf(stdout, NULL);
  printf("$ ");

  char input[MAX_INPUT_SIZE] = "";
  int pos = 0, tab_press_count = 0;
  char ch;

  while (1) {
    ch = getchar();

    if (ch == EOF || ch == 4)  {
      printf("\n");
      break;
    } else if (ch == '\n') {
      input[pos] = '\0';
      putchar('\n');

      char *trimmed_input = trim(input);
      if (*trimmed_input == '\0') {
        printf("$ ");
        continue;
      }
    
      parse_input(trimmed_input);
      evaluate(trimmed_input);
      free_globals();

      pos = 0;
      tab_press_count = 0;
      strcpy(input, "");
      printf("$ ");
    } else if (ch == 127 || ch == 8) {
      if (pos > 0) {
        input[--pos] = '\0';
        printf("\b \b");
      } 
    } else if (ch == '\t') autocomplete(input, &pos, &tab_press_count);
    else if (ch >= 32 && ch <= 126) {
      if (pos < MAX_INPUT_SIZE - 1) {
        input[pos++] = ch;
        putchar(ch);
      }
    }
  }

  disable_raw_mode(&orig_termios);
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