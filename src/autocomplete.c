#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "autocomplete.h"
#include "builtins.h"
#include "main.h"
#include "executables.h"
#include "utils.h"

void free_matches(Matches *matches) {
  if (!matches->words) return;

  for (int i = 0; i < matches->count; ++i) {
    free(matches->words[i]);
  }

  free(matches->words);
  matches->words = NULL;
  matches->count = 0;
}

char* find_longest_common_prefix(Matches *matches) {
  char *sample = matches->words[0];
  char *prefix = NULL;
  int prefix_idx = 0;

  while (1) {
    char compared_char = sample[prefix_idx];
    if (compared_char == '\0') break;

    int found_prefix_end = 0;
    for (int j = 1; j < matches->count; ++j) {
      if (matches->words[j][prefix_idx] != compared_char || matches->words[j][prefix_idx] == '\0') {
        found_prefix_end = 1;
        break;
      }
    }

    if (found_prefix_end) break;
    ++prefix_idx;
  }

  if (!prefix_idx) return NULL;

  prefix = malloc(prefix_idx + 1);
  if (!prefix) {
    perror("Prefix malloc failed");
    exit(1);
  }

  strncpy(prefix, sample, prefix_idx);
  prefix[prefix_idx] = '\0';

  return prefix;
}

void handle_single_match(char *input, int *pos, char *original_word, char *full_word, int with_ending_space) {
  int additional_len = strlen(full_word) - strlen(original_word);
  int full_word_padding = strlen(full_word) - additional_len;

  strncpy(input + *pos, full_word + full_word_padding, additional_len);
  input[*pos + additional_len] = ' ';
  input[*pos + additional_len + 1] = '\0';

  for (int i = 0; i < additional_len; ++i) {
    putchar(full_word[full_word_padding + i]);
  }

  if (with_ending_space) putchar(' ');

  *pos += additional_len;
  if (with_ending_space) *pos += 1;
}

void handle_multiple_matches(char *input, int *pos, char *original_word, Matches *matches, int *tab_press_count) {
  if (*tab_press_count == 1) {
    char *longest_common_prefix = find_longest_common_prefix(matches);
    if (!longest_common_prefix || strlen(longest_common_prefix) <= strlen(original_word)) printf("\a");
    else {
      handle_single_match(input, pos, original_word, longest_common_prefix, 0);
      *tab_press_count = 0;
    }

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

void complete_word(char *input, int *pos, char *original_word, Matches *matches, int *tab_press_count) {
  if (matches->count > 1) {
    handle_multiple_matches(input, pos, original_word, matches, tab_press_count);
    return;
  }

  handle_single_match(input, pos, original_word, matches->words[0], 1);
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
    free_matches(&matches);
    return;
  }

  printf("\a");
}

