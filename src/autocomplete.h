#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

typedef struct {
  char **words;
  int count;
} Matches;

void autocomplete(char *input, int *pos, int *tab_press_count);

#endif