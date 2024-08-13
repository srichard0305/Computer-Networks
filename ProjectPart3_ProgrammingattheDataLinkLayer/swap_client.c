/*
*	swap_client.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define	MAXLINE 128	// maximum characters to receive and send at once
#define	MAXFRAME 256

extern int swap_connect(unsigned int addr, unsigned short port);
extern int swap_disconnect(int sd);
extern int sdp_send(int sd, char *buf, int length);
extern int sdp_receive(int sd, char *buf);
extern int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration);
extern unsigned short checksum(unsigned char buf[], int length);

int session_id = 0;
int S = 0;	// frame number sent

int swap_open(unsigned int addr, unsigned short port)
{
	int	sockfd;		// sockect descriptor
	struct	sockaddr_in	servaddr;	// server address
	char	buf[MAXLINE];
	int	len, n;

	/*
	*	if the session is already open, then return error
	*/

	if (session_id != 0)
		return -1;

	/*
	*	connect to a server
	*/

	session_id = swap_connect(addr, port);	// in sdp.o

	/*
	*	return the seesion id
	*/

	return session_id;
}

int swap_write(int sd, char *buf, int length)
{
	int n;
	char frame[MAXFRAME], ackFrame[MAXFRAME];
  	unsigned short sendChecksum;

	if (session_id == 0 || sd != session_id)
		return -1;

	/*
	*	send a DATA frame
	*/
	
	//add sequence number to frame
	frame[0] = S;
	
	sendChecksum = checksum(buf, length);
	char first = sendChecksum >> 8;
	char second = sendChecksum & 0xff;
	frame[1] = first;
	frame[2] = second;
	
	// copy buffer content to frame 
	int i;
	for(i = 0; i < length; i++){
		frame[i+3] = buf[i];
	}
	
	while(true){
		
	     // send frame to server
	    printf("Sending frame to server\n");
	    n = sdp_send(sd, frame, length+3);
	
	    if(n == -1){
		    printf("General Error\n");
		    exit(1);
	    }
	

	  /*
	  *	read a frame with a timer
	  */
	
	
	    printf("Waiting for ACK message timer set....\n");
	    n =  sdp_receive_with_timer(sd, ackFrame, 3000);
	

	   /*
	   *	several different cases including disconnection
	   */
	
	
	    if(n == -1){
	
		   printf("General Error\n");
		   exit(1);
	
	   }
	
	   else if(n == -2){
		
		  printf("Session Disconnected\n");
		  exit(1);
	   }
	
	   //timer ran out 
	    else if(n == -3){
	        printf("Timer Ran out... Resending frame to server\n");
	        continue;
	   }
	   else {
	  
	      if(ackFrame[0] == ((S + 1) % 2) && 
	         ackFrame[1] == first &&
	         ackFrame[2] == second){
	   
	         printf("Ack message received no corruption\n");
	         S = (S + 1) % 2;
             break;
            }
			else
			   printf("Ack message is corrupted and discarded\n");
		}
	
	}
	
	return length + 3;
}

void swap_close(int sd)
{
	if (session_id == 0 || sd != session_id)
		return;

	else
		session_id = 0;

	swap_disconnect(sd);	// in sdp.o
}
