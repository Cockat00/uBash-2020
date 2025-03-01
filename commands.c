#include "library.h"


int count_args(char *str, char *delim){
	int num_arg = 0;
	char *token, *ptr;
	
	char *copy = malloc(strlen(str) + 1);
	if(copy == NULL) return -1;
	strcpy(copy,str);

	for(ptr=copy; ; ptr = NULL){
		token = strtok(ptr,delim);
		if(token == NULL) break;
		if(token[0] != '<' && token[0] != '>')
			num_arg++;
	}
	free(copy);
	return num_arg;
}


int seek_args(char *src, char **dest){
	char *delim = " ";
	int i = 0;

	char *token = strtok(src,delim);

	while(token){
		if(token[0] != '<' && token[0] != '>'){		// Ignoring redirection file, they're not arguments
			if(token[0] == '$' && token[1] != ' ' && token[1] != '\0'){
				if((dest[i++] = getenv(token + 1)) == NULL){
					fprintf(stderr, "%s: Variable not found\n", token);
					return -1;
				}
			}else dest[i++] = token;
		}
		token = strtok(NULL,delim);
	}
	dest[i] = NULL;	
	return 0;
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


char get_redir_type(char *cmd){
	char type = '\0';

	char *ptr = strchr(cmd,'<');
	if(ptr == NULL)
		ptr = strchr(cmd,'>');

	if(ptr) type = ptr[0];

	return type;
}


char *redir_IO(char *cmd){
	char *ptr = strchr(cmd,'<');
	if(ptr == NULL)
		ptr = strchr(cmd,'>');


	if(ptr != NULL){
		ptr++;

		for(int i = 0; i < strlen(ptr); ++i){
			if(ptr[i] == ' '){
				ptr[i] = '\0';
				break;
			}
		}
	}
	return ptr;
}


int manage_redir(char *redir, char type){
	int open_fd = -1;

	if(type == '<'){
		open_fd = open(redir, O_RDONLY);
		if(open_fd == -1){ 
			fail(redir);
			return -1;
		}
		_dup2(open_fd,STDIN_FILENO);
		close(open_fd);
	}

	if(type == '>'){
		open_fd = open(redir, O_WRONLY | O_CREAT | O_APPEND, S_IWUSR | S_IRUSR);
		if(open_fd == -1){
			fail(redir);
			return -1;
		}
		_dup2(open_fd,STDOUT_FILENO);
		close(open_fd);
	}

	return 0;
}

int execution(char *sub_cmd){
	int num_args = count_args(sub_cmd," ");

	char **cmd_args = (char **)malloc((num_args + 1) * sizeof(char*));
	if(seek_args(sub_cmd,cmd_args) == -1){
		free(cmd_args);
		return -1;
	} 

	if(execvp(cmd_args[0],cmd_args) == -1){
		fail(cmd_args[0]);
		free(cmd_args);
		return -1;
	}
	free(cmd_args);
	return 0;
}


void exec_sub_cmd(char **sub_cmd, char *big_input, int num_cmds){
	int pipes = -1, pipefd[2], fd_in = -1;
	char type = '\0';

	if(num_cmds > 1) pipes = 0;

	for(int j = 0; j < num_cmds; ++j){

		type = get_redir_type(sub_cmd[j]);

		if(pipes == 0){
			if(pipe2(pipefd,O_CLOEXEC) == -1) 
			fail_errno("exec_sub_cmd");	
		}

		pid_t pid = fork();
		if(pid == -1){
			free(big_input);
			fail_errno(sub_cmd[j]);
		}
		if(pid == 0){
			if(type){
				char *redir = redir_IO(sub_cmd[j]);
				if(manage_redir(redir,type) == -1){
					free(big_input);
					exit(EXIT_FAILURE);
				}
			}

			if(pipes == 0){
				if(fd_in > -1 && j > 0){
					_dup2(fd_in,STDIN_FILENO);
					close(fd_in);
				}
				if((j+1) < num_cmds) _dup2(pipefd[1],STDOUT_FILENO);
				close(pipefd[1]);
				close(pipefd[0]);
			}

			if(execution(sub_cmd[j]) == -1){
				free(big_input);
				exit(EXIT_FAILURE);
			}

			printf("\n");
		}else{

			if(pipes == 0){
				close(pipefd[1]);
				if(j > 0)
					close(fd_in);
				fd_in = pipefd[0];
				if((j + 1) == num_cmds)
					close(pipefd[0]);
			}

			int status;

			do{
				pid_t wpid = waitpid(pid,&status,WUNTRACED);

				if(wpid == -1) 
					fail_errno("waitpid()");

			#ifdef DEBUG
				child_status_handle(status);
			#endif
			}while(!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}
}


void exec_builtin(char *arg){

	if(strcmp(arg,ELARG) == 0){
		fprintf(stderr,"cd: %s.\n",ELARG);
		return;
	}

	if(strcmp(arg,EMARG) == 0){
		fprintf(stderr,"cd: %s.\n",EMARG);
		return;
	}

	if(strcmp(arg,CDREDIR) == 0){
		fprintf(stderr,"cd: %s.\n",CDREDIR);
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
