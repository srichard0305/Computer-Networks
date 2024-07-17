/*
 * COMP 3271 - Computer Networks
 * Project Part 1 - UDP Time Server - server implementation
 */

#include <stdio.h>		/* I/O functions */
#include <string.h>		/* string functions */
#include <stdlib.h>		/* C standard functions */
#include <sys/socket.h>	/* socket functions */
#include <sys/types.h>	/* library of basic types */
#include <netinet/in.h>	/* library of Internet address functions */
#include <arpa/inet.h>	/* Internet operations */
#include <time.h>		/* time functions */

#define PORT 9988		/* server port # */
#define BUFFSIZE 200	/* buffer size */

int main()
{
	int sockfd;
	int addrlen;
	char buffer[BUFFSIZE];
	struct sockaddr_in server;
	struct sockaddr_in client;
	char name[30];
	char studentID[30];
	time_t current_time;

	/* Populate socket data structures with IP address and port number */
	memset((char *) &server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	/* Create a UDP socket; returns -1 on failure */
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd == -1) {
		printf("Socket error\n");
		exit(1); /* Exit on error */
	}

	/* Bind the socket address */
	if ((bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr_in))) == -1) {
		printf("Server bind error\n");
		exit (1); /* Exit on error */
	}

	/* Helpful status message */
	printf("Server is listening on port: %d\n", PORT);
	printf("Waiting for client request...\n");
	printf("Press CTRL + C to exit\n");
	
	while(1) {
		addrlen = sizeof(struct sockaddr_in);
		
		/* Receive request for time and send back*/
		recvfrom(sockfd, buffer,BUFFSIZE, 0,(struct sockaddr *)&client, (socklen_t *)&addrlen);
		current_time = time(NULL);
		sendto(sockfd, (char *) &current_time, (int) sizeof(current_time), 0, (struct sockaddr *)&client, addrlen);
		
		/* Receive student name and echo back*/
		printf("\nReceiving Student Name\n");
		recvfrom(sockfd, (char *) &name,(int)sizeof(name), 0,(struct sockaddr *)&client, (socklen_t *)&addrlen);
		printf("Student Name: %s", name);
		sendto(sockfd, (char *) &name, (int) sizeof(name), 0, (struct sockaddr *)&client, addrlen);
		printf("\nEcho back Student Name\n");
		
		/* Receive studentID and echo back*/
		printf("\nReceiving Student ID\n");
		recvfrom(sockfd, (char *) &studentID,(int)sizeof(studentID), 0,(struct sockaddr *)&client, (socklen_t *)&addrlen);
		printf("Student ID: %s", studentID);
		sendto(sockfd, (char *) &studentID, (int) sizeof(studentID), 0, (struct sockaddr *)&client, addrlen);
		printf("\nEcho back Student ID\n");
	}
	exit(0);

} /* End of time server program */
