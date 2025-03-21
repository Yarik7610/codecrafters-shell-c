# Overview

In this challenge, i've built POSIX compliant shell (that's capable of
interpreting shell commands, running external programs and builtin commands like
"echo", "exit", "type", "pwd", "cd", and with other features listed below.

# Features

- exit builtin
- echo builtin
- type builtin
- pwd builtin
- cd builtin
- quoting (both single and double) for commands and arguments, but not for redirect files
- backslash support (outside / inside quotes)
- redirecting into custom output and error files, including appending mode
- autocompletion for builtins and executable commands (multiple matches will be printed as in bash, one match will be autocompleted)
- partial completion if there was found bigger common prefix of all matches

# How to run

1. Ensure you have `cmake` installed locally
2. Run `./your_program.sh` to run your program

# Comment

This task was pretty interesting to realize, the main goal of the project was to gain some practical skills in C after reading the original book. It was fun to use some C functions to interact with OS directly (chdir, termios, access, getenv, getpwd, exit, system). Big thanks CodeCrafters (https://codecrafters.io/) for a such challenge)  
