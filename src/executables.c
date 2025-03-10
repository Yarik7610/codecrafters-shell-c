#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <io.h> 
#include "executables.h"

#define MAX_FULL_PATH_LENGTH 1024
#define FILE_EXISTS 0

char* get_command_from_env_path(char *command) {
  char *path = getenv("PATH");

  if (!path) return NULL;

  char *path_copy = strdup(path);
  if (!path_copy) {
    perror("Path copy malloc failed");
    exit(1);
  }
 
  char *splitted_path = strtok(path_copy, ";");

  while (splitted_path != NULL) {
    char full_path[MAX_FULL_PATH_LENGTH]; 

    snprintf(full_path, MAX_FULL_PATH_LENGTH, "%s\\%s", splitted_path, command);

    if (_access(full_path, FILE_EXISTS) != -1) {
      free(path_copy);

      char *command_path = strdup(full_path);
      if (!command_path) {
        perror("Full path answer malloc failed");
        exit(1);
      }

      return command_path;
    }

    splitted_path = strtok(NULL, ";");
  }

  free(path_copy);

  return NULL;
}