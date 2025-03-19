#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> 
#include <dirent.h>
#include "executables.h"
#include "main.h"

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


char* find_uncompleted_in_executables(char *uncompleted, int uncompleted_len) {
  char *match = NULL;

  char *path_copy = get_env_path_copy();
  if (!path_copy) return NULL;

  char *splitted_path = strtok(path_copy, ":");

  while (splitted_path != NULL) {
    DIR *dir = opendir(splitted_path);
    if (!dir) {
      closedir(dir);
      splitted_path = strtok(NULL, ":");
      continue;
    }

    struct dirent *de;

    while ((de = readdir(dir)) != NULL) {
      if (strncmp(de->d_name, uncompleted, uncompleted_len) == 0) {
        match = strdup(de->d_name);
        if (!match) {
          perror("Directory entry name malloc failed");
          exit(1);
        }
        break;
      }
    }

    closedir(dir);

    if (match != NULL) break;

    splitted_path = strtok(NULL, ":");
  }

  free(path_copy);

  return match;
}

