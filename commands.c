#include "library.h"

char *error_check(const char *token){
	return "WTF";
}

void exec_builtin(char *arg){

	if(strcmp(arg,ELARG) == 0){
		printf("cd: Expected one argument.\n");
		return;
	}

	if(strcmp(arg,EMARG) == 0){
		printf("cd: Too many arguments.\n");
		return;
	}
	if(strcmp(arg,"~") == 0) 
		arg = getenv("HOME");
	    
	cd(arg);
}

void cd(char *new_path){
	if(chdir(new_path) == -1) 
		fail(new_path);
}
