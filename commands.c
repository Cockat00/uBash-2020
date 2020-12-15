#include "library.h"


int count_args(char *vect){
	int num_arg = 0;
	char *delim = " ";
	char *token, *str;
	
	char *copy = malloc(strlen(vect) + 1);
	if(copy == NULL) return -1;
	strcpy(copy,vect);

	for(str=copy; ; str = NULL){
		token = strtok(str,delim);
		if(token == NULL) break;
		num_arg++;
	}
	free(copy);
	return num_arg;
}


void seek_args(char *src, char **dest){
	char *delim = " ";
	int i = 0;

	char *token = strtok(src,delim);

	while(token){
		dest[i] = token;
		token = strtok(NULL,delim);
		i++;
	}
	dest[i] = NULL;

	return;	
}


void exec_ext(char *ext){
	pid_t pid;

	pid = fork();
	if(pid == -1) fail_errno("exec_ext");
	if(pid == 0){
		int num_args = count_args(ext);

		char **cmd_args = (char **)malloc((num_args + 1) * sizeof(char*));
		seek_args(ext,cmd_args);
		if(execvp(cmd_args[0],cmd_args) == -1) fail_errno(cmd_args[0]);

		printf("\n");
		free(cmd_args);
		
	}else{
		int status;
			if(waitpid(pid,&status,0) == -1) fail_errno("waitpid()");
			//if(strcmp("",cmd_and_args) != 0) free(cmd_and_args);

			if(WIFEXITED(status))
				printf("(%d) - exited with status %d\n",pid,WEXITSTATUS(status));
			else
				printf("Something went wrong\n");
	}
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
