//libraries
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>

//define your functions and your structs
void *compress(void* args);
void print(int count, char prev, FILE * name);


typedef struct fun_args{
  int num;
  int start;
  int end;
} Args;

//global variables
FILE * file;
int size;
const char * filename;


int main(int argc, char const *argv[])
{
  struct timeval start_time, end_time ;
  // file name and # of output files will be taken as arguments
  if ( argc != 3 ) {
    printf( "ERROR TRY AGAIN\n" );
    return 0;
  }
  else {
    filename = argv[1];
    FILE *file = fopen( filename, "r" );
    int outputs = strtol(argv[2], NULL, 10);
    
    if (outputs < 1){
      printf("Arg[2] VALUE MUST BE AT LEAST 1\n");
      return 0;
    }

    if (file==0){
      printf("YOU CANNOT ACCESS THIS FILE\n");
      return 0;
    }
    else {
      

      //find file size
      fseek(file, 0, SEEK_END);
      size = ftell(file);
      fseek(file, 0, SEEK_SET);
      fclose(file);

      
      printf("size:%d\n", size);

      if(outputs>size){
        printf("ERROR THE NUMBER YOU ENTERED IS TOO LARGE\n");
        return 0;
      }

      //array of thread names and properties
      pthread_t tid[outputs];
      Args argz[outputs];

      //now break up the string
      int component_size = size/outputs;
      int leftover = size%outputs;
      int i=0;

      //make those threads
      //printf("imma create mah threads now\n");
      printf("Creating threads. TIME START\n");
      gettimeofday(&start_time, NULL);

      while (i<outputs) {
        if(i==0){
          argz[i].start = 0;
          argz[i].end = component_size + leftover;
        }
        else {
          argz[i].start = (component_size * i) + leftover + 1;
          argz[i].end = argz[i].start + component_size - 1;
        }
        argz[i].num = i;
        pthread_create(&tid[i], NULL, compress, &argz[i]);
        i++;
      }

      //join 'em
      int k = 0;
      while (k<outputs){
        pthread_join(tid[k], NULL);
        k++;
      }
      //printf("mah threads are dun\n");
      printf("Threads finished. TIME STOP\n");
      gettimeofday(&end_time, NULL);
    }


  }

  float time;
  time = (end_time.tv_sec * 1000000 + end_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec);
  printf("\n%ld microseconds\n", ((end_time.tv_sec * 1000000 + end_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec)));
  return 0;
}

void* compress (void* args){

  FILE *file, *fp2;
  char ch;

  Args * ptr = args;
  int start = ptr->start;
  int end = ptr->end;
  int threadnum = ptr->num;

  //naming convention
  char write[30];
  if(threadnum != 1){
    sprintf(write, "%s_txt_LOLS%d", filename, threadnum);
  }
  else {
    sprintf(write,"%s_txt_LOLS", filename);
  }

  // gonna have to fix this name thing
  file = fopen("test.txt", "r");
  fp2 = fopen(write, "w");


  fseek(file, start, SEEK_SET);
  fseek(fp2, 0, SEEK_SET);

  int i, j;
  int count = 0;
  char curr = NULL;
  char prev = NULL;

  for (i=start-1; i<end+1; i++){
    count = 1;
    for (j=i+1; j<end+1; j++){
      prev = curr;
      curr = fgetc(file);
      
      if(isalpha(curr)){
        
        if(prev==curr){
          if(j==i+1 && i==start-1){
            continue;
          }
          count++;        
        } 
        else{
          print(count, prev, fp2);
          break;
        }
      }
      else {
        
          printf("\nNOT COMPRESSIBLE CHARACTER\n");
          continue;
  
      }
    }
    //prev = curr;
    i=j-1;
  }
  print(count, prev, fp2);
  printf("\n");
  fclose(file);
  fclose(fp2);
}

void print(int count, char prev, FILE * name){
  //printf("%s:  ", name);
  if(prev!=NULL){

    
    if (count>2){
      fprintf(name, "%d%c", count, prev);
      printf("%d%c", count, prev);
    } else if (count==2){
      fprintf(name, "%c%c", prev, prev);
      printf("%c%c", prev, prev);
    } else {
      fprintf(name, "%c", prev);
      printf("%c", prev);
    }
  }
  
}

