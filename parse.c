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
	int i = 0;

	char **cmd_list = (char **)malloc(num_cmd * sizeof(char *));
	if(cmd_list == NULL) fail_errno(ext_cmd);

	for(tmp = ext_cmd; ;tmp = NULL){
		token = strtok_r(tmp,"|",&saveptr);
		if(token == NULL) break;
		cmd_list[i++] = token;
	}

	for(int j = 0; j < num_cmd; ++j)
		exec_ext(cmd_list[j]);

	free(cmd_list);
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
	}else{
		parse_ext2(big_input);
	}
}


// Old parser

	/*for(str1 = cpy; ;str1 = NULL){
		token = strtok_r(str1,"|",&saveptr);
		if(token == NULL) break;
		if(parse_cmd(token,count) == -1)
			break;
	}*/

/*void parse_ext(char *ext_cmd){
	char *token = NULL, *tmp = NULL, *saveptr = NULL;
	int count = 0;		// Used for some error check (to be implemented soon)

	char *cpy = malloc(strlen(ext_cmd) + 1);
	if(cpy == NULL) fail_errno("parse_ext");
	strcpy(cpy,ext_cmd);

	for(tmp = cpy; ;tmp = NULL){
		token = strtok_r(tmp,"|",&saveptr);
		if(token == NULL) break;
		exec_ext(token);
		//printf("Actual: '%s' | Next: '%s'\n", token, saveptr);
		count++;
	}
	free(cpy);
}*/

/*With parse_cmd we'll be able to detect whenever
  a string refer to a builtin or an external command.*/
/*int parse_cmd(char *cmd, char *ptr, int count){
	char *saveptr = NULL;
	char *token = NULL;
	char *cmmd = NULL; //La prima stringa che definisce il comando da eseguire

	printf("%s\n", ptr);
	token = strtok_r(cmd," ",&saveptr);
	if(token){
		if(strcmp(token,"cd") == 0){ 
			cmmd = parse_builtin(saveptr,token);	//Start builtin command module
			exec_builtin(cmmd);
		} 
		else{
			cmmd = parse_ext(big_input);	
			if(cmmd == NULL) return -1;
			_dummy_method();
		}
	}
	return 0;
}*/
