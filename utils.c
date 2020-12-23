#include "library.h"

void fail_errno(char *arg){
	fprintf(stderr,"%s(%d): %s\n",arg,errno,strerror(errno));
	exit(EXIT_FAILURE);
}


void fail(char *arg){
	fprintf(stderr,"%s(%d): %s\n",arg,errno,strerror(errno));
}


int valid_cmd_check(char *cmd){
	if(strstr(cmd," cd ") == NULL) // To ensure that command with 'cd' substring will not be marked as not valid 
		return -1;

	return 0;
}

int char_cntrl(char ch){
	int res = -1;
	if(ch == '|' || ch == '<' || ch == '>') 
		res = 0;
	return res;
}

void _dup(int to_redir, int redir_fd){
	int dup_res = 0;

	dup_res = dup2(to_redir,redir_fd);

	if(dup_res == -1) fail_errno("_dup");
}
