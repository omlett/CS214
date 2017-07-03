#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
	// Error Check
	if (argc != 3){
		printf("Error Incorrect Input to compressR_LOLS\n");
		return 0;
	}
	
	FILE *sniped;
	pid_t parent = getpid();
	pid_t pid;
	char *splits = argv[2];
	int status;
	
	// Error Check
	const char *sniper = argv[1];
	sniped = fopen(sniper, "r");
	if (sniped == NULL){
		printf("error opening file \n");
		return 0;
	}
	else{
		pid = fork();
		if (pid == -1){
			printf("Error\n");
			return 0;
		}
		else if (pid == 0){
			execl("./compressR_worker_LOLS", "./compressR_worker_LOLS", sniper, "0", "0", splits, "0", (char *) NULL);
		}
		else{
			waitpid(pid, &status, WNOHANG);
			return 0;
		}
	}
}

