#include "library.h"

char *error_check(const char *token){
	return "WTF";
}

void cd(const char *new_path){
	if(chdir(new_path) == -1) 
		fail("cd");
}
