#include "library.h"

void fail_errno(char *arg){
	fprintf(stderr,"%s(%d): %s\n",arg,errno,strerror(errno));
	exit(EXIT_FAILURE);
}



void fail(char *arg){
	fprintf(stderr,"%s(%d): %s\n",arg,errno,strerror(errno));
}



int char_cntrl(char ch){
	int res = -1;
	if(ch == '|' || ch == '<' || ch == '>') 
		res = 0;
	return res;
}


void _dup2(int to_redir, int dest){
	if(dup2(to_redir,dest) == -1)
		fail_errno("_dup2");
	//fprintf(stderr,"child: [%d]  -->  [%d]\n", dest,to_redir);
}



int check_num_spaces(char *str){
	int count = 0;

	for(int i = 0; i < strlen(str); ++i){
		if(str[i] != ' ') 
			break;
		count++;
	}
	return count;
}
