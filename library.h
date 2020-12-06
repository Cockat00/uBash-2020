#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define STD_BUFSIZE 128
#define PIPE "|"
#define REDIN "<"
#define REDOUT ">"

typedef struct _command{
	bool pipes;
	bool builtin;
 }command;

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
