#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define STD_BUFSIZE 128

// Errors
#define ELARG "Too less arguments"							// Error Less ARGuments
#define EMARG "Too many arguments"							// Error More ARGuments
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
int check_blank_spaces(char str[]);
char *get_input();
void uBash();


// parse.c
void parse_input(char *arg);
char *parse_builtin(char *token, char *saveptr);
void parse_ext(char *ext_cmd);
	

//commands.c
void cd(char *path);
void exec_builtin(char *arg);
void exec_ext(char *ext);
