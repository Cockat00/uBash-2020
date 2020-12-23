#include "library.h"

/*Simple method used as a debug purpose*/
void _dummy_method(){
	printf("Dummy reached\n");
}



char *parse_builtin(char *token, char *saveptr){
	char *res = NULL;

	token = strtok_r(NULL," ",&saveptr);
	res = token;

	if(res == NULL) return ELARG;

	while(token){
		token = strtok_r(NULL," ",&saveptr);
		if(token != NULL)
			//TODO: Scan argument type and give proper error message
			return EMARG;
	}
	return res;	
}



void parse_ext2(char *ext_cmd){
	char *token = NULL, *tmp = NULL, *saveptr = NULL;
	int num_cmd = count_args(ext_cmd,"|");
	char *cmd_list[num_cmd];
	int i = 0;

	for(tmp = ext_cmd; ;tmp = NULL){
		token = strtok_r(tmp,"|",&saveptr);
		if(token == NULL) break;
		cmd_list[i++] = token;
	}
	exec_ext(cmd_list,ext_cmd,num_cmd);
}



/*parse_input's duty is to search for syntax errors and 
  then define whenever the input refer to a builtin or
  an external command.*/ 
void parse_input(char *big_input){

	if(char_cntrl(big_input[0]) == 0){
		fprintf(stderr, "Invalid usage of token %c\n", big_input[0]);
		return;
	}

	if(char_cntrl(big_input[strlen(big_input) - 1]) == 0){
		fprintf(stderr, "Invalid usage of token %c\n", big_input[strlen(big_input) - 1]);
		return;
	}

	char builtin[2];
	builtin[0] = big_input[0];
	builtin[1] = big_input[1];

	char *cmmd = NULL;

	if(strcmp("cd",builtin) == 0){
		char *token = NULL, *saveptr = NULL;
		token = strtok_r(big_input," ",&saveptr);	// Consume 'cd' token
		cmmd = parse_builtin(token,saveptr);	
		exec_builtin(cmmd);
	}else 
		parse_ext2(big_input);
}
