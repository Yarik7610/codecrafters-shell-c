#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "autocomplete.h"
#include "builtins.h"
#include "main.h"
#include "executables.h"

void complete_word(char *input, int *pos, char *original_word, char *full_word) {
  int additional_len = strlen(full_word) - strlen(original_word);
  int full_word_padding = strlen(full_word) - additional_len;

  strncpy(input + *pos, full_word + full_word_padding, additional_len);
  input[*pos + additional_len] = ' ';
  input[*pos + additional_len + 1] = '\0';

  for (int i = 0; i < additional_len; ++i) {
    putchar(full_word[full_word_padding + i]);
  }

  putchar(' ');

  *pos += additional_len + 1;
}

void autocomplete(char *input, int *pos) {
  int uncompleted_start_idx = *pos - 1;

  while (uncompleted_start_idx >= 0 && !isspace(input[uncompleted_start_idx])) uncompleted_start_idx--;
  uncompleted_start_idx++;

  int uncompleted_len = *pos - uncompleted_start_idx;
  if (uncompleted_len <= 0) return;

  char uncompleted[MAX_INPUT_SIZE];
  strncpy(uncompleted, input + uncompleted_start_idx, uncompleted_len);
  uncompleted[uncompleted_len] = '\0';

  char *match = NULL;
  int match_needs_free = 0;

  match = find_uncompleted_in_builtins(uncompleted, uncompleted_len);
  if (match) {
    complete_word(input, pos, uncompleted, match); 
    return;
  }
 
  match = find_uncompleted_in_executables(uncompleted, uncompleted_len);
  if (match) {
    complete_word(input, pos, uncompleted, match); 
    free(match);
    return;
  }

  printf("\a");
}