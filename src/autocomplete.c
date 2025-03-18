#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "autocomplete.h"
#include "builtins.h"
#include "main.h"

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
  int word_start_idx = *pos - 1;

  while (word_start_idx >= 0 && !isspace(input[word_start_idx])) word_start_idx--;
  word_start_idx++;

  int word_len = *pos - word_start_idx;
  if (word_len == 0) return;

  char word_buffer[MAX_INPUT_SIZE];
  strncpy(word_buffer, input + word_start_idx, word_len);
  word_buffer[word_len] = '\0';

  char *match = NULL;
  
  for (int i = 0; builtins[i] != NULL; ++i) {
    if (strncmp(word_buffer, builtins[i], word_len) == 0) {
      match = builtins[i];
      complete_word(input, pos, word_buffer, match);
      return;
    }
  }

  printf("\a");
}

