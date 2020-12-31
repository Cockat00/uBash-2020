#include "library.h"


char *parse_builtin(char *token, char *saveptr){
	char *res = NULL;

	token = strtok_r(NULL," ",&saveptr);
	res = token;

	if(res == NULL) return ELARG;

	while(token){
		token = strtok_r(NULL," ",&saveptr);
		if(token != NULL)
			return EMARG;
	}
	return res;	
}


int find_cmd(char *cmd){
	char *path = getenv("PATH");
	char *sub_path = NULL;
	size_t length = 0;
	char *ptr = NULL;

	if(!path) return -1;

	do{
		ptr = strchr(path,':');
		if(ptr == NULL) ptr = path + strlen(path);

		length = ptr - path;
		sub_path = malloc(length + strlen(cmd) + 2);
		strncpy(sub_path,path,length);
		sub_path[length] = '\0';
		if(access(strcat(strcat(sub_path,"/"),cmd),F_OK) == 0){
			free(sub_path);
			return 0;
		}

		if(*ptr == ':') ptr++;

		free(sub_path);
		path = ptr;
	}while(ptr != path + strlen(path));
	return -1;
}


/*Taking the first substring of the command (logically the command itself)
  check_cmd_type will return an integer, which indicate if the command
  is a builtin (1), valid (0) or invalid (-1)*/
int check_cmd_type(char *sub_cmd){
	int res = 0;
	char *token = NULL;
	char *cpy = malloc(strlen(sub_cmd) + 1);
	strcpy(cpy,sub_cmd);

	token = strtok(cpy," ");

	if(strcmp(token,"cd") == 0) res = 1;
	else{
		if(find_cmd(token) == -1)
			res = -1;
	}
	free(cpy);
	return res;
}


int validate_pipe_pos(char *str, int index, int num_cmd){

	if(index == 0  &&  str[strlen(str) - 1] != ' ')
		return -1;
	
	if((index + 1) == num_cmd  &&  str[0] != ' ')
		return -1;
	
	if((index + 1) < num_cmd  &&  index > 0){
		if(str[0] != ' ' || str[strlen(str) - 1] != ' ')
			return -1;
	}
	return 0;
}

//TODO: Please Refactoring in a fancier way

/*The method search through the sub_command for syntax and semantic errors.
  It will return 0 if the sub command is valid, -1 if not, 1 if it is 
  a builtin command. If 1 is returned, *cmd will be parsed with builtin semantic rules*/
int valid_cmd_check(char *cmd, int index, int num_cmd){

	if(num_cmd > 1){
		if(validate_pipe_pos(cmd,index,num_cmd) == -1){
			fprintf(stderr, "%s\n", E_PIPE);
			return -1;
		}
	}

	if(check_num_spaces(cmd) == strlen(cmd)){
		fprintf(stderr, "Cannot execute a void command\n");
		return -1;
	}

	int cmd_type = check_cmd_type(cmd);

	if(cmd_type == 1){
		if(num_cmd > 1){
			fprintf(stderr, "Cannot use a builtin command in this context\n");
			return -1;
		} return 1;
	}else if(cmd_type == -1){
		fprintf(stderr, "'%s' Does not exist\n",cmd);
		return -1;
	}

	char *ptr = NULL;

	ptr = strchr(cmd,'>');
	if(ptr != NULL){
		if(num_cmd > 1){
			if((index + 1) < num_cmd){
				fprintf(stderr,"%s\n",EREDOUT);
				return -1;
			}
		}

		if(ptr[1] == ' '){
			fprintf(stderr, "Cannot find redirection argument\n");
			return -1;
		}
	}

	ptr = strchr(cmd,'<');
	if(ptr != NULL){
		if(num_cmd > 1){
			if(index > 0){
				fprintf(stderr,"%s\n",EREDIN);
				return -1;
			}
		}

		if(ptr[1] == ' '){
			fprintf(stderr, "Cannot find redirection argument\n");
			return -1;
		}
	}
	return 0;
}



void parse_cmd(char *big_input){
	char *sub_cmd = NULL, *tmp = NULL, *saveptr = NULL;
	int num_cmd = count_args(big_input,"|");
	char *cmd_list[num_cmd];
	int i = 0, res = 0, builtin = -1;

	for(tmp = big_input; ;tmp = NULL){
		sub_cmd = strtok_r(tmp,"|",&saveptr);
		if(sub_cmd == NULL) break;

		res = valid_cmd_check(sub_cmd,i,num_cmd);

		if(res == -1) return;

		if(res == 1) builtin = 0;
	 	cmd_list[i++] = sub_cmd;
	}


	if(builtin == 0){
		char *token = NULL, *saveptr = NULL, *new_dir = NULL;
		token = strtok_r(cmd_list[0]," ",&saveptr);	// Consume 'cd' token
		new_dir = parse_builtin(token,saveptr);	
		exec_builtin(new_dir);
	}else 
		exec_sub_cmd(cmd_list,big_input,num_cmd);

}



void parse_input(char *big_input){

	if(char_cntrl(big_input[0]) == 0){
		fprintf(stderr, "Invalid usage of token %c\n", big_input[0]);
		return;
	}

	if(char_cntrl(big_input[strlen(big_input) - 1]) == 0){
		fprintf(stderr, "Invalid usage of token %c\n", big_input[strlen(big_input) - 1]);
		return;
	}

	parse_cmd(big_input);
}
