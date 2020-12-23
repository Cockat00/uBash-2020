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
}



void child_status_handle(int status){
	if (WIFEXITED(status)) 
		printf("child exited (%d)\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status)) 
   		printf("child killed (%d)\n", WTERMSIG(status));
    else if (WIFSTOPPED(status))
    	printf("child stopped (%d)\n", WSTOPSIG(status));
    else printf("Unknown error\n");
}



void exec_ext(char **ext, char *big_input, int num_cmd){
	int pipes = -1;
	int pipefd[2];
	int fd_in = 0;

	if(num_cmd > 1) pipes = 0;

	for(int j = 0; j < num_cmd; ++j){

		if(pipes == 0){
			if(pipe2(pipefd,O_CLOEXEC) == -1) 
			fail_errno("exec_ext");	
		}

		pid_t pid = fork();
		if(pid == -1) fail_errno("exec_ext");
		if(pid == 0){
			if(pipes == 0){
				if(fd_in != 0) _dup(fd_in,STDIN_FILENO);
				if((j + 1) < num_cmd) _dup(pipefd[1],STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
			}

			int num_args = count_args(ext[j]," ");

			char **cmd_args = (char **)malloc((num_args + 1) * sizeof(char*));
			seek_args(ext[j],cmd_args);
			if(execvp(cmd_args[0],cmd_args) == -1){
				fail(cmd_args[0]);
				free(cmd_args);
				free(big_input);		// WHY THIS WORK??!!
				exit(EXIT_FAILURE);
			}
			free(cmd_args);
			printf("\n");
		}else{

			int status;
			int wpid;

			while((wpid = waitpid(pid,&status,WUNTRACED)) > 0){
				if(pipes == 0){
					close(pipefd[1]);
					fd_in = pipefd[0];
					if(j >= num_cmd)
						close(pipefd[0]);
				}

				if(wpid == -1) 
					fail_errno("waitpid()");

			#ifdef DEBUG
				child_status_handle(status);
			#endif
			}
		}
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
