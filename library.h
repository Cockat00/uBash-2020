#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define STD_BUFSIZE 128

typedef struct {
	int n_pipes;
	bool built_in;
	char *big_input;
}command;

// utils.c
void fail_errno(void *arg);

// shell.c
char *get_input();
void uBash();

// parse.c

