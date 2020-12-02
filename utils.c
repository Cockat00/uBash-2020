#include "library.h"

void fail_errno(void *arg){
	fprintf(stderr,"%p(%d): %s\n",arg,errno,strerror(errno));
	exit(EXIT_FAILURE);
}
