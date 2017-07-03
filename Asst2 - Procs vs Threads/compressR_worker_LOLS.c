#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>



int main(int argc, char *argv[]){
	if (argc != 6){
		printf("Error Incorrect Input to Worker\n");
		return 0;
	}

	FILE *sniped;			// File Handle for input file
	int c;				// used to count total_char
	int total_chars;		// Total characters in input file
	int total_read = 0;		// Track characters read
	int written = 0;		// Track characters written
	int start;			// Start Compression
	int stop;			// Stop Compression
	int splits;			// Subfiles to make
	int split_count;		// Track subfiles
	int chars_split;		// Characters per split
	int step;			// Track where worker is
	char buffer[2048];		// Buffer for reading
	char next_file[64];		// Maintain output filenames
	char scout[64];			// Get name of file minus extension
	int status;
	
	const char *sniper = argv[1];
	start = atoi(argv[2]);
	stop = atoi(argv[3]);
	splits = atoi(argv[4]);
	step = atoi(argv[5]);

	// Eliminate .txt
	int convert;
	for (convert = 0; convert < strlen(sniper)-4; convert++){
		scout[convert] = sniper[convert];	
	}
	scout[convert+1] = '\0';

	// Open File, Error Check
	sniped = fopen(sniper, "r");
	if (sniped == NULL){
		printf("Error Opening File\n%s\n", sniper);
		return 0;
	}
	else{
		// Run on first fork
		if (step == 0){			
			// Calculate total_chars
			fseek(sniped, 0, SEEK_END);
			total_chars = ftell(sniped) - 1;
			fseek(sniped, 0, SEEK_SET);

			// Calculate chars_split, set initial start, stop
			chars_split = (total_chars / splits);
			if ((chars_split*splits) < total_chars){
				start = 0;
				stop = chars_split + (total_chars - (chars_split*splits));
			}
			else{
				start = 0;
				stop = chars_split;
			}
		}
		
		if (step >= 1){
			int counter = 0;

			sprintf(next_file, "%s_txt_LOLS%d", scout, (step-1));
			FILE *bombsite = fopen(next_file, "w+");
			fseek(sniped, start, SEEK_SET);

			
			/*
			total_read += fread(buffer, sizeof(char), stop-start, sniped);
			while (total_read < (stop-start)){
				total_read += fread(buffer, sizeof(char), stop-start, sniped);
			}
			*/

			// RLE
			int i, j; 
  			int count = 0;
  			char curr = NULL;
			char prev = NULL;
  		
  			for (i = (start-1); i < (stop+1); i++){
  				count = 1;
  				for (j = (start+1); j < (stop+1); j++){
  					curr = fgetc(sniped);
					if ((isalpha(curr)) || (curr == '\n') || (curr == EOF)){	
  						if (prev == curr){
							if(j==i+1 && i==start-1){
           							continue;
							}
  							count++;  			
  						}
  						else{
							if (count > 2){
								fprintf(bombsite, "%d%c", count, prev);
							}
							else if (count == 2){
								fprintf(bombsite, "%c%c", prev, prev);
							}
							else{
								if (prev != NULL){
									fprintf(bombsite, "%c", prev);
								}
							}
							break;
  						}
					}
					else{
						
					}
  				}
				prev = curr;
  				i = j-1;
  			}
/*
			if (count > 2){
				fprintf(bombsite, "%d%c", count, curr);
			}
			else if (count == 2){
				fprintf(bombsite, "%c%c", curr, curr);
			}
			else{
				fprintf(bombsite, "%c", curr);
			}
*/
			fclose(bombsite);
			return 0;
		}

		// Loop and fork until desired splits are completed
		while(step < splits){
			step++;
			pid_t parent = getpid();
			pid_t pid = fork();
		
			if (pid == -1){
				printf("Error Forking\n");
			}
			else if (pid == 0){
				// Convert Arguments to String before Routing them to execl()
				char start_ac[64], stop_ac[64], splits_ac[64], step_ac[64];
				sprintf(start_ac, "%d", start);
				sprintf(stop_ac, "%d", stop);
				sprintf(splits_ac, "%d", splits);
				sprintf(step_ac, "%d", step);
				execl("./compressR_worker_LOLS", "./compressR_worker_LOLS", sniper, start_ac, stop_ac, splits_ac, step_ac, (char *) NULL);
			}
			else{
				//waitpid(pid, &status, WNOHANG);
				start = stop;
				stop += chars_split;
			}
		}
	}
	fclose(sniped);
	return 0;
}
