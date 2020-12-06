#include "library.h"

void cd(const char *new_path){
	if(chdir(new_path) == -1) 
		fail("cd");
}
