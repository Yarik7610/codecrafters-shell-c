#ifndef MAIN_H
#define MAIN_H

#define MAX_INPUT_SIZE 100
#define MAX_FULL_PATH_LENGTH 1024

extern char *flags;
extern int flags_count;

extern char **arguments;
extern int arguments_count;

typedef struct {
  char *path;
  char mode[4];
} RedirectFileInfo;

extern RedirectFileInfo redirect_out_info;
extern RedirectFileInfo redirect_err_info;

#endif