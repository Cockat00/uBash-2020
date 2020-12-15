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
