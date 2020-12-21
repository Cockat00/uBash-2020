#include "library.h"


int count_args(char *vect, char *delim){
	int num_arg = 0;
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


int seek_args(char *src, char **dest){
	char *delim = " ";
	int i = 0;

	char *token = strtok(src,delim);

	if(strcmp("cd",token) == 0){
		fprintf(stderr, "Invalid use of '%s' builtin command", token);
		return -1;
	}

	while(token){
		dest[i] = token;
		token = strtok(NULL,delim);
		i++;
	}
	dest[i] = NULL;

	return 0;	
}


void exec_ext(char **ext, int num_cmd){

	pid_t pid;
	int pipes = -1;
	int pipefd[2];
	int wpid;

	if(num_cmd > 1){
		if(pipe2(pipefd,0) == -1) fail_errno("exec_ext");
		pipes = 0;
	}

	for(int j = 0; j < num_cmd; j++){
		pid = fork();
		if(pid == -1) fail_errno("exec_ext");
		if(pid == 0){

			if(pipes == 0){
				
				if((j - 1) >= 0)
					_dup2(pipefd[0],pipefd[1],STDIN_FILENO);

				if((j + 1) < num_cmd)
					_dup2(pipefd[1],pipefd[0],STDOUT_FILENO);
			}

			int num_args = count_args(ext[j]," ");

			char **cmd_args = (char **)malloc((num_args + 1) * sizeof(char*));
			if(seek_args(ext[j],cmd_args) == 0){
				if(execvp(cmd_args[0],cmd_args) == -1) 
					fail(cmd_args[0]);
			}
		
			printf("\n");
			free(cmd_args);
		}
	}

	if(pipes == 0){
		close(pipefd[0]);
		close(pipefd[1]);
	}

	int status;

	while((wpid = waitpid(-1,&status,0)) > 0){
		//TODO: Extends exit status check
		if(wpid == -1) fail_errno("waitpid()");

		if(WIFEXITED(status))
			printf("(%d) - exited with status %d\n",pid,WEXITSTATUS(status));
		else
			printf("Something went wrong\n");
	}
	
		
	/*pid_t pid;

	pid = fork();
	if(pid == -1) fail_errno("exec_ext");
	if(pid == 0){
		int num_args = count_args(ext," ");

		char **cmd_args = (char **)malloc((num_args + 1) * sizeof(char*));
		if(seek_args(ext,cmd_args) == 0){
			if(execvp(cmd_args[0],cmd_args) == -1) 
				fail(cmd_args[0]);
		}
		
		printf("\n");
		free(cmd_args);
	}else{
		int status;
			//TODO: Extends exit status check
			if(waitpid(pid,&status,0) == -1) fail_errno("waitpid()");

			if(WIFEXITED(status))
				printf("(%d) - exited with status %d\n",pid,WEXITSTATUS(status));
			else
				printf("Something went wrong\n");
	}*/
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
