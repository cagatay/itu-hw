#include <stdio.h>
#include <sys/syscall.h>

#define __NR_mycall 333

int main(int argc, char **argv){

	if(argc < 3){
		printf("Usage: test pid switch\n");
		return 0;
	}

	int result;
	int pid = atoi(argv[1]);
	int sw = atoi(argv[2]);

	result = syscall(__NR_mycall, pid, sw);
	
	// Interprete error codes.
	if(result == -1)
		printf("Invisibility couldn't set.\nTry running as root.\n");
	else if(result == 1)
		printf("Try with different pid.\n");
	else if(result == 2)
		printf("Switch must be 0 (off) or 1 (on)\n");
	
	return 0;
}
