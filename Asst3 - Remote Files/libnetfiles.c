#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int init = -1;

int netserverinit(struct hostent * serverIPAddress){	
	if (serverIPAddress == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		printf("No such host\n");
		init = -1;
	}else {
		init = 0;
	}
	return init;
}


void error(char *msg){
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[]){
	// Declare initial vars
	int count = 0;
	int sockfd = -1;																
	int portno = -1;																
	int n = -1;																		
	char buffer[300];																
	struct sockaddr_in serverAddressInfo;											
	struct hostent *serverIPAddress;

	if (argc < 3){
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);
	serverIPAddress = gethostbyname(argv[1]);

   

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		error("ERROR creating socket");
	}

	// zero out the socket address info struct .. always initialize!
	bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	serverAddressInfo.sin_family = AF_INET;	
	serverAddressInfo.sin_port = htons(portno);
	bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);

	// try to connect to the server using our blank socket 
	if (connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) < 0) {
		error("ERROR connecting");
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////// if we got this far that means we connected ////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////



	printf("we connected\n");

	while (1){
		if (count < 2){
			printf("Please enter the function you wish to run: \ninit(i), open(o), read(r), write(w) or close(c)?\n");
			bzero(buffer,300);
			fgets(buffer,100,stdin);

			if (buffer[0]!= 'i' && buffer[0]!= 'o' && buffer[0]!= 'r' && buffer[0]!= 'w' && buffer[0]!= 'c'){
				printf("BAD INPUT\n");
				exit(0);
			}

			if(buffer[0]=='i'){
				netserverinit(serverIPAddress);
			
				printf("Please enter the function you wish to run: \nopen(o), read(r), write(w) or close(c)?\n");
				bzero(buffer,300);
				fgets(buffer,100,stdin);
				
				count = 2;
			}
				
			if (buffer[0]!= 'o' && buffer[0]!= 'r' && buffer[0]!= 'w' && buffer[0]!= 'c'){
				printf("you POS you can't do anything right\n");
				exit(0);
			}
		}
		if (init == -1){
			printf("don''t forget to set the h_eerno\n");
			return -1;
		}
		if (count > 2){
			printf("Please enter the function you wish to run: \nopen(o), read(r), write(w) or close(c)?\n");
			bzero(buffer,300);
			fgets(buffer,100,stdin);
		}

		

		buffer[0] = tolower(buffer[0]);
		char temp[5];
		int g;

		// let's get some more information
		switch(buffer[0]){
			case 'o':
				printf("enter r, w, or rw... lowercase letters only\n");
				fgets(&temp[0], 5, stdin);

				switch (temp[0]){
					case 'r':
						if (temp[1]=='w'){
							buffer[1] = '3';
						}
						else {
							buffer[1] = '1';
						}
						break;
					case 'w':
						buffer[1]='2';
						break;
					default:
						printf("incorrect input\n");
						exit(0);
				}

				printf("Enter pathname\n");
				fgets(&buffer[2], 200, stdin);

				if(buffer[2] == '\0'){
					printf("you did not enter a pathname\n");
					exit(0);
				}

				break;

			case 'r':
				printf("enter file descriptor\n");
				fgets(&buffer[1], 200, stdin);

				int x = 0;
				while(buffer[x] != '\0'){
					x++;
				}
				buffer[x] = '|';
				x++;

				if (buffer[1] == '\0'){
					printf("you did not enter the file descriptor\n");
					exit(0);
				}

				printf("enter size in bytes of each element to be read\n");
				fgets(&buffer[x], 100, stdin);

				if (buffer[x] == '\0'){
					printf("you did not enter a size\n");
					exit(0);
				}

				break;

			case 'w':
				printf("enter file descriptor\n");
				fgets(&buffer[1], 100, stdin);

				x = 0;
				while(buffer[x] != '\0'){
					x++;
				}
				buffer[x] = '|';
				x++;

				if (buffer[1] == '\0'){
					printf("you did not enter the file descriptor\n");
					exit(0);
				}

				printf("enter size in bytes of each element to be written\n");
				fgets(&buffer[x], 50, stdin);

				if (buffer[x] == '\0'){
					printf("you did not enter a size\n");
					exit(0);
				}

				while(buffer[x+1] != '\0'){
					x++;
				}
				buffer[x] = '|';
				x++;

				printf("enter what you want to be written\n");
				fgets(&buffer[x], 150, stdin);

				if (buffer[x] == '\0'){
					printf("you did not enter anything\n");
					exit(0);
				}

				break;

			case 'c':
				printf("enter file descriptor\n");
				fgets(&buffer[1], 300, stdin);

				if (buffer[1] == '\0'){
					printf("you did not enter the file descriptor\n");
					exit(0);
				}

				break;

			default:
				printf("an error has occured\n");
				exit(0);

		}

		//write to server

		char funct = buffer[0];
		
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0){
			error("ERROR writing to socket");
		}

		bzero(buffer,300);

		n = read(sockfd,buffer,300);
		if (n < 0){
			error("ERROR reading from socket");
		}
		
		int i=0;


		char returned[20];
		for(g=0; buffer[g]!='\0'; g++){
			returned[i] = buffer[i];
			printf("%c", buffer[g]);
		}

		int returns;
		ssize_t ret;
		
		switch(funct){
			case 'o':
				//returned value is an int
				returns = atoi(returned);
				break;
				//return returns;
			case 'r':
				//returned number of bytes read (ssize_t)
				ret = atoi(returned);
				//return ret;
				break;
			case 'w':
				//returned number of bytes written (ssize_t)
				ret = atoi(returned);
				//return ret;
				break;
			case 'c':
				//returned number
				returns = atoi(returned);
				//return returns;
				break;
			default:
				return -1;
		}

		//printf("Would you like to continue (Y/N)?\n");
		//bzero(buffer, 300);
		//fgets(buffer, 5, stdin);
	}


	

	count++;

	return 0;



}
