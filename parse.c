#include "library.h"


void _dummy_method(){
	printf("Dummy reached\n");
}

////////BUILTIN FUNCTIONS START/////////

char *parse_builtin(char *saveptr, char *token){
	char *res = NULL;

	token = strtok_r(NULL," ",&saveptr);
	res = token;

	if(res == NULL) return "ELARG";

	while(token){
		token = strtok_r(NULL," ",&saveptr);
		if(token != NULL){
			//TODO: Scan argument type and give proper error message
			return "EMARG";
		}
	}
	return res;	
}

void exec_builtin(char *arg){

	if(strcmp(arg,"ELARG") == 0){
		printf("cd: Expected one argument.\n");
		return;
	}

	if(strcmp(arg,"EMARG") == 0){
		printf("cd: Too many arguments.\n");
		return;
	}
		if(strcmp(arg,"~") == 0) 
			arg = getenv("HOME");
	    
	    cd(arg);
}

////////BUILTIN FUNCTIONS END/////////

struct command *__init_command(struct command *arg){
	arg -> pipes = false;
	arg -> builtin = false;
	return arg;
}


void parse_cmd(char *cmd){
	char *saveptr = NULL;
	char *token = NULL;
	char *cmmd = NULL; //La prima stringa che definisce il comando da eseguire

	if(cmd == NULL) return; //Saltiamo se ci troviamo tra due pipe senza alcun carattere

	token = strtok_r(cmd," ",&saveptr);
	if(token){
		if(strcmp(token,"cd") == 0){ 
			cmmd = parse_builtin(saveptr,token);
			exec_builtin(cmmd);
		} 
		else _dummy_method();
	}
}


/*parse_input's duty is to wrap the big input in token
 using '|' as a delimiter. Each token will be treated 
 as a single command and parsed as external or builtin command*/ 
void parse_input(char *big_input){
	char *token = NULL,*saveptr = NULL,*str1 = NULL;

	char *cpy = malloc(strlen(big_input) + 1);
	if(cpy == NULL) fail_errno("parse_input");
	strcpy(cpy,big_input);

	for(str1 = cpy; ;str1 = NULL){
		token = strtok_r(str1,"|",&saveptr);
		if(token == NULL) break;
		printf("%s\n",token);

		parse_cmd(token);
	}
	free(cpy);
}
