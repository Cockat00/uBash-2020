#include "library.h"

/*Starting from a STD_BUFSIZE long buffer, get_input()
 * allocate the correct amount of byte (indicated by buf_len)
 * input require*/
char *get_input(){
	char buf[STD_BUFSIZE];
	char *input = NULL;

	if(fgets(buf,STD_BUFSIZE,stdin) != NULL){
		size_t buf_len = strlen(buf);
		if(buf_len <= 1) return "";	//if buf = "\n\0"
		if(buf[buf_len - 1] == '\n')
			buf[buf_len - 1] = '\0';

		input = malloc(buf_len);
		if(input == NULL) return "";
		strcpy(input,buf);
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
			//printf("%s\n",big_input);	
			free(big_input);
		}	
	}	
}
