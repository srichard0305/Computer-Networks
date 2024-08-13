#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define	MAXLINE	125	// maximum bytes to receive and send at once

// External functions
extern int swap_wait(unsigned short port);
extern int swap_read(int sd, unsigned char buf[]);
extern void swap_close(int sd);

int main(int argc, char *argv[])
{
	unsigned short server_port;
	unsigned int server_address;
	unsigned char message[MAXLINE];
	int sd, n, out;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(1);
	}

	// set values for server address and port number
	server_port = htons(atoi(argv[1]));

	// connect to the port allocated to the swap server
	sd = swap_wait(server_port);
	if (sd < 0) {
		fprintf(stderr, "swap_open error\n");
		exit(0);
	}

	//read in the first message as the new name of the file
	n = swap_read(sd, message);  
        if(n < 0){
	  printf("General Error\n");
	  exit(1); 
	}
	
	//open output file for writing
	printf("Opening file %s for writing....\n", message);
	out = open(message, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
	if(out < 0){
	  printf("Cannot create file\n");
	  exit(1); 
	}
	
	int bytesWritten;
	printf("Reading input from client and writing to file...\n");
	while(1){
	  n = swap_read(sd, message);
	  
	  if(n < 0){
	    printf("General Error\n");
	    exit(1); 
	  }
	  
	  if(n < sizeof(message)){
	    bytesWritten = write(out, message, n);
	    break;
	  }
	
	  bytesWritten = write(out, message, n);
	}
	printf("Contents written to file.\n");
	
		  
	// close the connection to the swap server
	printf("Closing swap\n");
	swap_close(sd);
	
	//close connection to output file
	printf("Closing file\n");
	close(out);
}
