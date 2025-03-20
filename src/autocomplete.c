#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "autocomplete.h"
#include "builtins.h"
#include "main.h"
#include "executables.h"
#include "utils.h"

void free_match_words(Matches *matches) {
  if (!matches->words) return;

  for (int i = 0; i < matches->count; ++i) {
    free(matches->words[i]);
  }

  free(matches->words);
}



void handle_multiple_matches(char *input, int *pos, char *original_word, Matches *matches, int *tab_press_count) {
  if (*tab_press_count == 1) {
    printf("\a");
    return;
  } 
  
  qsort(matches->words, matches->count, sizeof(char *), compare_strings);

  printf("\n");

  for (int i = 0; i < matches->count; ++i) {
    printf("%s", matches->words[i]);
    if (i != matches->count - 1) printf("  ");
  }

  printf("\n$ %s", input);

  --(*tab_press_count);
}

void handle_single_match(char *input, int *pos, char *original_word, char *full_word) {
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

void complete_word(char *input, int *pos, char *original_word, Matches *matches, int *tab_press_count) {
  if (matches->count > 1) {
    handle_multiple_matches(input, pos, original_word, matches, tab_press_count);
    return;
  }

  handle_single_match(input, pos, original_word, matches->words[0]);
  *tab_press_count = 0;
}

void autocomplete(char *input, int *pos, int *tab_press_count) {
  ++(*tab_press_count);

  int uncompleted_start_idx = *pos - 1;

  while (uncompleted_start_idx >= 0 && !isspace(input[uncompleted_start_idx])) uncompleted_start_idx--;
  uncompleted_start_idx++;

  int uncompleted_len = *pos - uncompleted_start_idx;
  if (uncompleted_len <= 0) return;

  char uncompleted[MAX_INPUT_SIZE];
  strncpy(uncompleted, input + uncompleted_start_idx, uncompleted_len);
  uncompleted[uncompleted_len] = '\0';

  Matches matches = {NULL, 0};

  find_uncompleted_in_builtins(uncompleted, uncompleted_len, &matches);
  if (!matches.words) find_uncompleted_in_executables(uncompleted, uncompleted_len, &matches);
  if (matches.words) {
    complete_word(input, pos, uncompleted, &matches, tab_press_count); 
    free_match_words(&matches);
    return;
  }

  printf("\a");
}

