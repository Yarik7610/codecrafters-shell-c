#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> 
#include <dirent.h>
#include "executables.h"
#include "main.h"
#include "autocomplete.h"

char* get_env_path_copy() {
  char *path = getenv("PATH");
  if (!path) return NULL;

  char *path_copy = strdup(path);
  if (!path_copy) {
    perror("Path copy malloc failed");
    exit(1);
  }

  return path_copy;
}

char* get_command_from_env_path(char *command) {
  char *path_copy = get_env_path_copy();
  if (!path_copy) return NULL;

  char *splitted_path = strtok(path_copy, ":");
  char full_path[MAX_FULL_PATH_LENGTH]; 

  while (splitted_path != NULL) {
    snprintf(full_path, MAX_FULL_PATH_LENGTH, "%s/%s", splitted_path, command);

    if (access(full_path, X_OK) != -1) {
      free(path_copy);

      char *command_path = strdup(full_path);
      if (!command_path) {
        perror("Full path answer malloc failed");
        exit(1);
      }

      return command_path;
    }

    splitted_path = strtok(NULL, ":");
  }

  free(path_copy);

  return NULL;
}

int is_in_matches(char *word, Matches *matches) {
  int in_matches = 0;
  
  for (int i = 0; i < matches->count; ++i) {
    if (strcmp(matches->words[i], word) == 0) {
      in_matches = 1;
      break;
    }
  }

  return in_matches;
}

void find_uncompleted_in_executables(char *uncompleted, int uncompleted_len, Matches *matches) {
  char *path_copy = get_env_path_copy();
  if (!path_copy) return;

  char *splitted_path = strtok(path_copy, ":");

  while (splitted_path != NULL) {
    DIR *dir = opendir(splitted_path);
    if (!dir) {
      splitted_path = strtok(NULL, ":");
      continue;
    }

    struct dirent *de;

    while ((de = readdir(dir)) != NULL) {
      if (strncmp(de->d_name, uncompleted, uncompleted_len) == 0) {
        int in_matches = is_in_matches(de->d_name, matches);
        if (in_matches) continue;

        matches->words = realloc(matches->words, (matches->count + 2) * sizeof(char *));
        if (!matches->words) {
          perror("Executable matches realloc failed");
          exit(1);
        }

        char *match = strdup(de->d_name);
        if (!match) {
          perror("Executable match malloc failed");
          exit(1);
        }

        matches->words[matches->count++] = match;
        matches->words[matches->count] = NULL;
      }
    }

    closedir(dir);
    splitted_path = strtok(NULL, ":");
  }

  free(path_copy);
}

