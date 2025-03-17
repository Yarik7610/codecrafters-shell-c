#ifndef MAIN_H
#define MAIN_H

#define MAX_INPUT_SIZE 100
#define MAX_FULL_PATH_LENGTH 1024

extern char *flags;
extern int flags_count;

extern char **arguments;
extern int arguments_count;

extern char *stdout_file_path;
extern char stdout_file_mode[4];

extern char *stderr_file_path;
extern char stderr_file_mode[4];

#endif