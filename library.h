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
#define EREDIN "Invalid usage of character '<'"
#define EREDOUT "Invalid usage of character '>'"
#define E_PIPE "Invalid usage of pipe"

#ifdef DEBUG
	# define DEBUG_PRINT printf
#endif

struct strCmd{
	char *cmd;
	char *redir;
	int red_in;
	int red_out;
};


// utils.c
void fail_errno(char *arg);
void fail(char *arg);
int char_cntrl(char ch);
void _dup(int to_redir, int redir_fd);
void _dup2(int to_redir, int dest);


// shell.c
int check_num_spaces(char str[]);
char *get_input();
void uBash();


// parse.c
void parse_input(char *arg);
char *parse_builtin(char *token, char *saveptr);
void parse_cmd(char *ext_cmd);
int valid_cmd_check(char *cmd, int index, int num_cmd);
	

//commands.c
void cd(char *path);
void exec_builtin(char *arg);
void exec_sub_cmd(char **ext, char *big_input, int num_cmd);
int count_args(char *vect, char *delim);
