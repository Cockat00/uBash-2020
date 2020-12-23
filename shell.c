#include "library.h"


int check_blank_spaces(char str[]){
	int count = 0;

	for(int i = 0; i < strlen(str); ++i){
		if(str[i] == ' ') count++;
		if(str[i] != ' ') break;
	}
	return count;
}

/*Starting from a STD_BUFSIZE long buffer, get_input()
 * allocate the correct amount of byte (indicated by buf_len)
 * input require*/
char *get_input(){
	char buf[STD_BUFSIZE];
	char *input = NULL;
	int offset = 0;

	if(fgets(buf,STD_BUFSIZE,stdin) != NULL){
		size_t buf_len = strlen(buf);
		if(buf[buf_len - 1] == '\n'){
			buf[buf_len - 1] = '\0';
			buf_len -= 1;
		}

		offset = check_blank_spaces(buf);	// We'll not consider blank spaces at beginning

		if((buf_len - offset) == 0) return "";

		input = malloc((buf_len - offset) + 1);
		if(input == NULL) return "";
		strcpy(input,buf + offset);
	}
	return input;
}


void uBash(){
	char *path = NULL;
	char *big_input = NULL;

	path = getcwd(NULL,0);	//getcwd will use malloc() to allocate the correct amount of bytes 
	if(path == NULL) fail_errno(path);
	printf("[%s]$ ",path);
        free(path);

	big_input = get_input();

	if(big_input == NULL){
		printf("\n");
		exit(EXIT_SUCCESS);
	}else{
		if(strcmp(big_input,"") > 0){
			parse_input(big_input);	
			free(big_input);
		}	
	}	
}
