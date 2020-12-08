#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define STD_BUFSIZE 128
#define E_PIPE "Invalid presence of pipe"
#define EREDIN "Invalid presence of input redirection"
#define EREDOUT "Invalid presence of output redirection"

struct command{
	bool pipes;
	bool builtin;
 };

// utils.c
void fail_errno(char *arg);
void fail(char *arg);

// shell.c
char *get_input();
void uBash();

// parse.c
void parse_input(char *arg);
char *parse_builtin(char *saveptr, char *token);
void exec_builtin(char *arg);
		

//commands.c
void cd(const char *path);
char *error_check(const char *token);
