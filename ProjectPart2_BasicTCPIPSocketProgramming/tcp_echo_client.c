// TCP echo client program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define	BUFLEN	512

int main (int argc, char* argv[ ])		// Three arguments to be checked later
{
	struct sockaddr_in servAddr;		// Server socket address data structure
	char *servIP = argv[1];				// Server IP address from command line
	int servPort = atoi(argv[2]);		// Server port number from command line
	char *message = argv[3];			// Message specified on the command line

	// Check for correct number of command line arguments
	if(argc != 4) {                                               
		printf("tcp-echo-client [IP address] [Port] [Message]\n");                             
		exit (1);
	}                                                

	// Populate socket address for the server
	memset (&servAddr, 0, sizeof(servAddr));		// Initialize data structure
	servAddr.sin_family = AF_INET; 					// This is an IPv4 address
	servAddr.sin_addr.s_addr = inet_addr(servIP); 	// Server IP address
	servAddr.sin_port = servPort;					// Server port number
	
	// Create a TCP socket stream
	int sock;
	if ((sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error: socket creation failed!\n");
		exit (1);
	}
	else
        printf("Socket successfully created..\n"); 

	// Connect  to the server
	if ((connect (sock, (struct sockaddr*)&servAddr, sizeof(servAddr))) == -1) {
		printf("Error: connection to the server failed!\n");
		exit (1);
	}
	 else
        printf("Connected to the server..\n"); 
	
	// Send data to the server...
	send(sock, message, sizeof(message), 0);
	
        char buffer[BUFLEN];
        char *buff_ptr = buffer;
        int num_of_bytes, length_of_message;
        int max_length = sizeof(buffer);
	
	// Receive data back from the server..
	//Loop while receiving data... 
	while((num_of_bytes = recv(sock, buff_ptr, max_length, 0)) > 0){
	    
	    buff_ptr += num_of_bytes; // move pointer along buffer
	    max_length -= num_of_bytes; //adjust max number of bytes
	    length_of_message += num_of_bytes; // update length of the message received 
	}
        buffer[length_of_message] = '\0';
        printf("Echo Message Received\n");
        printf("%s\n", buffer);
        
	// Close socket
	close (sock);
	
	// Stop program
	exit (0);
	
} // End main
