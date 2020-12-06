#include "library.h"


void _dummy_method(){
	printf("Dummy reached\n");
}

////////BUILTIN FUNCTIONS START/////////

char *parse_builtin(char *saveptr, char *token){
	char *res = NULL;

	token = strtok_r(NULL," ",&saveptr);
	res = token;
	while(token){
		token = strtok_r(NULL," ",&saveptr);
		if(token != NULL){
			//TODO: Scan argument type and give proper error message
			printf("cd: Too many arguments\n");
			return "ENUMARG";
		}
	}
	return res;	
}

void exec_builtin(char *arg){
	//TODO: (optional) Set a default home path in case arg == NULL or ~
	
	if(strcmp(arg,"ENUMARG") != 0)
	       	cd(arg); // Asolutamente temporanea, richiede una gestione migliore..
}

////////BUILTIN FUNCTIONS END/////////


// !!TEMPORANEA (SARA' SUDDIVISA IN PIU' METODI)!!
void parse_input(char *big_input){
	char *token = NULL;
	char *saveptr = NULL;
	char *cd_arg = NULL;

	token = strtok_r(big_input," ",&saveptr);
	if(strcmp(token,"cd") == 0){
		cd_arg = parse_builtin(saveptr,token);
		exec_builtin(cd_arg);
	}
}
