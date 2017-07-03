#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void step_a(){
	int x;
	char *y[3001];
	char *z;
	for (x = 0; x < 3000; x++){
		y[x] = (char *)mymalloc(sizeof(char));
	}
	for (x = 0; x < 3000; x++){
		myfree(y[x]);
	}
}

void step_b(){
	int x;
	char *y;

	for (x = 0; x < 3000; x++){
		y = (char *)mymalloc(sizeof(char));
		myfree(y);
	}
}

void step_c(){
	int x, r, count;
	char *y[6000];
	count = 1;
	for (x = 0; x < 6000; x++){
		srand(time(NULL));
		r = rand();
		if ((r % 2) == 0){
			y[count] = (char *)mymalloc(sizeof(char));
			count++;
		}
		else{
			if (count > 1){
				count--;
				myfree(y[count]);
			}
		}
	}

}

void step_d(){
	int x, r, count;
	char *y[6000];
	count = 1;

	for (x = 0; x < 6000; x++){
		srand(time(NULL));
		r = rand() % 5000;

		if ((r % 2) == 0){
			y[count] = (char *)mymalloc(r*sizeof(char));
			count++;
		}
		else{
			if (count > 1){
				count--;
				myfree(y[count]);
			}
		}
	}
}

void step_e(){
	struct Node{
		int x;
		struct Node *next;
	};
	
	char *y;
	y = (char *)mymalloc(sizeof(struct Node));
	y = y+1;
	myfree(y);
	y = y-499;
	myfree(y);
	y = y+7000;
	myfree(y);
}

void step_f(){
	int x;
	int *y;
	int **twoD = mymalloc(sizeof(int)*100);
	if (twoD != NULL){
		for (x = 0; x < 100; x++){
			twoD[x] = mymalloc(sizeof(twoD[x])*10);
		}
	}

	for (x = 0; x < 100; x++){
		y = twoD[x];
		myfree(y);
	}
	myfree(twoD);
}

int main() {
	struct timeval start, stop;
	int runs;
	long run_time;
	
	run_time = 0;
	for (runs = 0; runs < 2; runs++){
		gettimeofday(&start, NULL);
		step_a();
		gettimeofday(&stop, NULL);
		run_time += (stop.tv_usec - start.tv_usec);
		printf("RunTime for Run#%d: %d\n", runs, run_time);
	}
	printf("Workload A: Avg RunTime (microseconds): %d\n", (run_time/runs));
	
	run_time = 0;
	for (runs = 0; runs < 100; runs++){
		gettimeofday(&start, NULL);
		step_b();
		gettimeofday(&stop, NULL);
		run_time += (stop.tv_usec - start.tv_usec);
	}
	printf("Workload B: Avg RunTime (microseconds): %d\n", (run_time/runs));
	
	run_time = 0;
	for (runs = 0; runs < 100; runs++){
		gettimeofday(&start, NULL);
		step_c();
		gettimeofday(&stop, NULL);
		run_time += (stop.tv_usec - start.tv_usec);
	}
	printf("Workload C: Avg RunTime (microseconds): %d\n", (run_time/100));
	
	run_time = 0;
	for (runs = 0; runs < 100; runs++){
		gettimeofday(&start, NULL);
		step_d();
		gettimeofday(&stop, NULL);
		run_time += (stop.tv_usec - start.tv_usec);
	}
	printf("Workload D: Avg RunTime (microseconds): %d\n", (run_time/100));
	
	run_time = 0;
	for (runs = 0; runs < 100; runs++){
		gettimeofday(&start, NULL);
		step_e();
		gettimeofday(&stop, NULL);
		run_time += (stop.tv_usec - start.tv_usec);
	}
	printf("Workload E: Avg RunTime (microseconds): %d\n", (run_time/100));
	
	run_time = 0;
	for (runs = 0; runs < 100; runs++){
		gettimeofday(&start, NULL);
		step_f();
		gettimeofday(&stop, NULL);
		run_time += (stop.tv_usec - start.tv_usec);
	}
	printf("Workload F: Avg RunTime (microseconds): %d\n", (run_time/100));
	
	return 0;
}
