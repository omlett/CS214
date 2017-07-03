#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
	struct timeval start_time, end_time;
	int x;
	float running_total = 0;
	float average_time = 0;
	
	for (x = 0; x < 100; x++){
		gettimeofday(&start_time, NULL);
		execl("./compressR_worker_LOLS", "./compressR_worker_LOLS", "t2.txt", "0", "0", 2, "0", (char *) NULL);
		gettimeofday(&end_time, NULL);
		running_total += (end_time.tv_sec * 1000000 + end_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec);
	}
	
	average_time = running_total/100;

	printf("Average for 100 Runs of multiprocessing: \n%f\n", average_time);
	
	return 0;
}
