#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define STD_BUFSIZE 128

// Errors
#define ELARG "Too less arguments"							// Error Less ARGuments
#define EMARG "Too many arguments"							// Error More ARGuments
#define EREDIN "Invalid presence of input redirection"
#define EREDOUT "Invalid presence of output redirection"

#ifdef DEBUG
	# define DEBUG_PRINT(fmt,...) \
				do { if (DEBUG) fprintf(stdout, fmt, __VA_ARGS__); } while (0)
#endif


// utils.c
void fail_errno(char *arg);
void fail(char *arg);
int char_cntrl(char ch);
void _dup(int to_redir, int redir_fd);
int valid_cmd_check(char *cmd);


// shell.c
int check_blank_spaces(char str[]);
char *get_input();
void uBash();


// parse.c
void parse_input(char *arg);
char *parse_builtin(char *token, char *saveptr);
void parse_ext2(char *ext_cmd);
	

//commands.c
void cd(char *path);
void exec_builtin(char *arg);
void exec_ext(char **ext, char *big_input, int num_cmd);
int count_args(char *vect, char *delim);
