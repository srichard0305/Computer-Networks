//TCP Echo server program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define	BUFLEN	512											// Maximum length of buffer
#define PORT	8899										// Fixed server port number

int main (void)
{
	struct sockaddr_in server_address;						// Data structure for server address
	struct sockaddr_in client_address;						// Data structure for client address
	int client_address_len = 0;
		
	
	// Populate socket address for the server
	memset (&server_address, 0, sizeof (server_address));	// Initialize server address data structure
	server_address.sin_family = AF_INET; 					// Populate family field - IPV4 protocol
	server_address.sin_port = PORT;							// Set port number
	server_address.sin_addr.s_addr = INADDR_ANY;			// Set IP address to IPv4 value for localhost
	 	
	// Create a TCP socket; returns -1 on failure
	int listen_sock;
	if ((listen_sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error: Listen socket failed!\n");
		exit (1);
	}
	else
		printf("Socket Created Successfully!\n");

	// Bind the socket to the server address; returns -1 on failure
	if ((bind(listen_sock, (struct sockaddr *)&server_address, sizeof (server_address))) == -1) {
		printf("Error: binding failed!\n");
		exit (1);
	}
	else
		printf("Socket binding Successful!\n");
	
	// Listen for connections...
	if(listen(listen_sock, 5) < 0){
		printf("Error: Listening Failed!\n");
		exit(1);
	}
	else
		printf("Listening Successfully!\n");
		
	// Accept connections from client...
	client_address_len = sizeof(client_address);
	int sock_dis = accept(listen_sock, (struct sockaddr *) &client_address, &client_address_len);
	if(sock_dis < 0){
		printf("Error: Accepting Failed!\n");
		exit(1);
	}
	else
		printf("Server accepted the client\n");
	
	
	// Receive and format the data...
	char buffer[BUFLEN];
	
	int length_of_message = recv(sock_dis, buffer, BUFLEN, 0);
	printf("%s\n", buffer);
	
	// Echo data back to the client...
	send(sock_dis, buffer, sizeof(buffer), 0);
		
	close (listen_sock);// Close descriptor referencing server socket
	
} // End main
