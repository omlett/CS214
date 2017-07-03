#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

ssize_t size, ret;
int filed;
int sockfd = -1;														
int newsockfd = -1;														
int portno = -1;														
int clilen = -1;														
int n = -1;																
char output[300];

void error(char *msg)
{
    perror(msg);
    exit(1);
}

//MAH FUHKSHIONS
int netopen(const char *pathname, int flags){
	/////////////////
	printf("in netopen :)\n");

	filed = -1;
	switch (flags){
		case 1:
			filed = open(pathname, O_RDONLY);
			break;
		case 2:
			filed = open(pathname, O_WRONLY);
			break;
		case 3:
			filed = open(pathname, O_RDWR);
			break;
		default:
			printf("illegal input error");
			exit(-1);
	}
	return filed;
}

ssize_t netread(int fildes, void *buf, size_t nbyte){
	/////////////////
	printf("in netread :)\n");

	size=-1;
	size = read(fildes, buf, nbyte);
	return size;
}

ssize_t netwrite(int fildes, const void *buf, size_t nbyte){
	/////////////////
	printf("in netwrite :)\n");

	size = -1;
	size = write(fildes, buf, nbyte);
	return size;
}

int netclose(int fd){
	/////////////////
	printf("in netclose :)\n");

	ret = close(fd);
	return ret;
}

void *tread(void* sockd){
	int sockuse = *(int*)sockd;
	char buffer[300];

	while(1){
	bzero(buffer,300);

	n = read(sockuse,buffer,255);
	if (n < 0){
		error("ERROR reading from socket");
	}

	int flag, filepointer, filedes, size, numread, closeint;
	int i = 0;
	int j = 0;
	char ph[100];
	bzero(ph, 100);
	ssize_t numwritten;


	switch(buffer[0]){
		case 'o':
			while (buffer[i]!= '\0'){
				ph[i-2] = buffer[i];
				i++;
			}

			flag = atoi(&buffer[1]);

			filepointer = netopen(ph, flag);
			printf("%d\n", filepointer);

			//return file descriptor
			
			sprintf(output, "%d", filepointer);

			//return output;
			break;

		case 'r':
			for(i=1; buffer[i] != '|'; i++){
				ph[i-1] = buffer[i];
			}

			filedes = atoi(ph);
//			printf("pathname: %d\n", filedes);
			bzero(ph, 100);

			i++;
			for(j=0; buffer[i]!= '\0'; j++){
				ph[j] = buffer[i];
				i++;
			}

			size = atoi(ph);

//			printf("size: %d\n", size);

			bzero(buffer,300);
			numread = netread(filedes, buffer, size);

			sprintf(output, "%d", numread);
			//return output;
			break;

		case 'w':
			for(i=1; buffer[i] != '|'; i++){
				ph[i-1] = buffer[i];
			} 
			i++;

			filedes = atoi(ph);
//			printf("pathname: %d\n", filedes);

			bzero(ph, 100);
			for(j=0; buffer[i]!= '|'; j++){
				ph[j] = buffer[i];
				i++;
			}
			i++;

			size = atoi(ph);
//			printf("size: %d\n", size);

			bzero(ph, 100);
			for(j=0; buffer[i]!= '\0'; j++){
				ph[j] = buffer[i];
//				printf("%c", ph[j]);
				i++;
			}

			numwritten = netwrite(filedes, ph, size);

			if (numwritten <= size){
				sprintf(output, "%d", numwritten);
			} else {
				printf("there has been an error writing\n");
				exit(-1);
			}
			//return output;
			break;

		case 'c':
			for(i=1; buffer[i] != '\0'; i++){
				ph[i-1] = buffer[i];
//				printf("%c", ph[i-1]);
			} 

			filedes = atoi(ph);
//			printf("pathname: %d\n", filedes);
			closeint = netclose(filedes);

			//return 
			sprintf(output, "%d", closeint);
			//return output;
			break;

		default:
			printf("my bad\n");
			exit(1);
	}

    n = write(sockuse,output,18);
    if (n < 0){
		error("ERROR writing to socket");
	}

	}
}

int main(int argc, char *argv[]){

	// Variables
	bzero(output, 300);
	struct sockaddr_in serverAddressInfo;									
	struct sockaddr_in clientAddressInfo;
	int client_socket, *new_sock;					

	// Error Check
	if (argc < 2){
        	fprintf(stderr,"ERROR, no port provided\n");
        	exit(1);
    	}

    	portno = atoi(argv[1]);

	// First Call to socket() 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0){
		error("ERROR opening socket");
	}

	// Initialize Socket
	bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));
	serverAddressInfo.sin_port = htons(portno);
	serverAddressInfo.sin_family = AF_INET;
	serverAddressInfo.sin_addr.s_addr = INADDR_ANY;

	// Bind socket
	if (bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0){
		error("ERROR on binding");
	}	

//	printf("binding done\n");

	// Listen
	listen(sockfd,5);
	clilen = sizeof(clientAddressInfo);

	while(client_socket = accept(sockfd, (struct sockaddr *) &clientAddressInfo, (socklen_t*) &clilen)){

		pthread_t snifferid;
		new_sock = malloc(sizeof(int));
		*new_sock = client_socket;

		if ((pthread_create(&snifferid, NULL, tread, (void *) new_sock)) < 0){
			perror("could not create thread");
			return 1;
		}
		//pthread_join(snifferid, NULL);
	}

	if (client_socket < 0){
		perror("connection unaccepted");
		return 1;
	}

	return 0;
}













