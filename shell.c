#include "library.h"

void trim_end_spaces(char *buf, size_t buf_len){
	if(buf_len == 0)
		return;

	for(int i = buf_len - 1; i >= 0; i--){
		if(buf[i] != ' '){
			buf[i + 1] = '\0';
			break;
		}
		buf_len--;
	}
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
		buf[buf_len - 1] = '\0';
		buf_len--;

		offset = check_num_spaces(buf);

		if((buf_len - offset) == 0) return "";

		trim_end_spaces(buf,buf_len);

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
