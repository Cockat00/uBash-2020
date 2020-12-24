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


int check_builtin(char *sub_cmd){
	int res = -1;
	char *token = NULL;

	char *cpy = malloc(strlen(sub_cmd));
	if(cpy == NULL) fail_errno("check_builtin");
	strcpy(cpy,sub_cmd);

	token = strtok(cpy," ");

	if(strcmp(token,"cd") == 0) res = 0;

	free(cpy);
	return res;
}


//TODO: Please Refactoring in a fancier way
int valid_cmd_check(char *cmd, int index, int num_cmd){
	char *ptr = NULL;

	if(check_builtin(cmd) == 0){
		if(num_cmd > 1){
			fprintf(stderr, "Cannot use a builtin command in this context\n");
			return -1;
		} return 1;
	} 

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
				fprintf(stderr,"%s\n",EREDOUT);
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
		char *token = NULL, *saveptr = NULL, *cmmd = NULL;
		token = strtok_r(cmd_list[0]," ",&saveptr);	// Consume 'cd' token
		cmmd = parse_builtin(token,saveptr);	
		exec_builtin(cmmd);
	}else 
		exec_ext(cmd_list,big_input,num_cmd);
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
