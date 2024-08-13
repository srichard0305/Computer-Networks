#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define	MAXLINE	125	// maximum bytes to receive and send at once

// External functions
extern int swap_open(unsigned int addr, unsigned short port);
extern int swap_write(int sd, unsigned char buf[], int length);
extern void swap_close(int sd);

int main(int argc, char *argv[])
{
	unsigned short server_port;
	unsigned int server_address;
	unsigned char buf[MAXLINE];
	int sd, n, in;

	if (argc < 4) {
		fprintf(stderr, "Usage: %s address port input-filename\n", argv[0]);
		exit(1);
	}
	
	// set values for server address and port number
	server_address = inet_addr(argv[1]);	// server address
	server_port = htons(atoi(argv[2]));		// port number

	// connect to the swap server
	sd = swap_open(server_address, server_port);
	if (sd < 0) {
		fprintf(stderr, "swap_open error\n");
		exit(0);
	}
	
	//open input file for reading
	printf("Opening file %s for reading...\n", argv[3]);
	in = open(argv[3], O_RDONLY); 
	if(n < 0){
	  printf("Cannot open file %s\n", argv[3]); 
	  exit(1); 
	}
	
	//send output file name to the server
	printf("Sending output file name to server\n");
	n = swap_write(sd, argv[4], strlen(argv[4])); 
	if(n < 0){
	  printf("General Error\n");
	  exit(1); 
	}
      	
      	// loop through file and send to server
      	printf("Reading input from file and sending to server...\n");
	int bytesRead;
	while(1){
	  bytesRead = read(in, buf, sizeof(buf)); 
	  
	  if(bytesRead < sizeof(buf)){
	    n = swap_write(sd, buf, bytesRead);
	    if(n < 0){
	      printf("General Error\n");
	      exit(1); 
	    }
	    break;
	  }
	  
	  n = swap_write(sd, buf, bytesRead);
	  if(n < 0){
	    printf("General Error\n");
	    exit(1); 
	  }
        }
        printf("Contents of file have been written and sent to server.\n");
        
        
      
	// close the connection to the swap server
	printf("Closing swap\n");
	swap_close(sd);

	//close connection to input file
	printf("Closing file\n");
	close(in);
}
