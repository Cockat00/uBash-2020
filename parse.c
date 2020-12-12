#include "library.h"

/*Simple method used as a debug purpose*/
void _dummy_method(){
	printf("Dummy reached\n");
}


struct command *__init_command(struct command *arg){
	arg -> pipes = false;
	arg -> builtin = false;
	return arg;
}


char *parse_builtin(char *saveptr, char *token){
	char *res = NULL;

	token = strtok_r(NULL," ",&saveptr);
	res = token;

	if(res == NULL) return ELARG;

	while(token){
		token = strtok_r(NULL," ",&saveptr);
		if(token != NULL){
			//TODO: Scan argument type and give proper error message
			return EMARG;
		}
	}
	return res;	
}


char *parse_ext(char *saveptr, char *token){
	return "(null)";
}

/*With parse_cmd we'll be able to detect whenever
  a string refer to a builtin or an external command.*/
int parse_cmd(char *cmd, char *ptr, int count){
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
			cmmd = parse_ext(saveptr,token);	
			if(cmmd == NULL) return -1;
			_dummy_method();
		}
	}
	return 0;
}


/*parse_input's duty is to wrap the big input in token
 using ' ' as a delimiter. The first token define the behaviour
 the program will have*/ 
void parse_input(char *big_input){
	char *token = NULL,*saveptr = NULL;

	if(big_input[0] == '|'){
		fprintf(stderr, "Invalid usage of token %c\n", big_input[0]);
		return;
	}

	char *cpy = malloc(strlen(big_input) + 1);
	if(cpy == NULL) fail_errno("parse_input");
	strcpy(cpy,big_input);

	char *cmmd = NULL;
	token = strtok_r(cpy," ",&saveptr);

	if(strcmp("cd",token) == 0){
		cmmd = parse_builtin(saveptr,token);	//Start builtin command module
		exec_builtin(cmmd);
	}else _dummy_method();
	
	free(cpy);
}


// Old parser

	/*for(str1 = cpy; ;str1 = NULL){
		token = strtok_r(str1,"|",&saveptr);
		if(token == NULL) break;
		if(parse_cmd(token,count) == -1)
			break;

		count++;
	}*/
