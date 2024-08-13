/*
*	swap_server.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define	MAXLINE	128	// maximum characters to receive and send at once
#define MAXFRAME 256

extern int swap_accept(unsigned short port);
extern int swap_disconnect(int sd);
extern int sdp_send(int sd, char *buf, int length);
extern int sdp_receive(int sd, char *buf);
extern int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration);
extern unsigned short checksum(unsigned char buf[], int length);

int session_id = 0;
int R = 0;	// frame number to receive

int swap_wait(unsigned short port)
{
	/*
	*	if the session is already open, then return error
	*/

	if (session_id != 0)
		return -1;

	/*
	*	accept a connection
	*/

	session_id = swap_accept(port);	// in sdp.o

	/*
	*	return a ssession id
	*/

	return session_id;
}

int swap_read(int sd, char *buf)
{
	int n, i;
	char frame[MAXFRAME];
	unsigned recvChecksum;

	if (session_id == 0 || sd != session_id)
		return -1;

	/*
	*	receive a frame without a timer
	*/
	while(1){
	    
		printf("Receiving Frame\n");
	    n = sdp_receive(sd, frame);
	
	    if(n == -1){
	
		    printf("General Error\n");
		    exit(1);
	
	    }
	
	    if(n == -2){
		
		    printf("Session Disconnected\n");
		    exit(1);
	    }
	
	
	/* 
	*	copy the data field in the frame into buf, and return the length
	*/
	
	    //copy message into buffer
	    for(i = 0; i < n-3; i++){
		    buf[i] = frame[i+3];
	    }
	
	   //checksum
	    recvChecksum = checksum(buf, n-3);
	    char first = recvChecksum >> 8;
	    char second = recvChecksum & 0xff;
		
	    //if sequence number does not match clear buffer 
	    // send ack message with R 
	    if(R != frame[0]){
		     
			 printf("Sequence number did not matach\n");
		     for(i = 0; i < n-3; i++){
			      buf[i] = 0;			
		     }
		
		     frame[0] = R;
	
		     printf("Sending ack message with correct sequence number\n");
		     n = sdp_send(sd, frame, n);
	
		     continue;
	    }
	
	    //if checksums dont match clear buffer
	    // dont send ack message so timer runs out 
	    else if(frame[1] != first && frame[2] != second){
			
		     printf("Checksum did not match\n");
		     printf("No ack message sent\n");
			 
		     for(i = 0; i < n-3; i++){
			     buf[i] = 0;			
		     }
			 
		     continue; 
	    }
		
	    //if no errors with checksum or sequence number
	    else{
	       
		   //set next sequence number;
	       R = (R + 1) % 2;
	
	       //add sequence number and checksum to send back 
	       frame[0] = R;
	  
	       printf("Sending ack message that frame was received with out issue\n");
	       //send ack message
	       sdp_send(sd, frame, n);
	
	       return n-3;
	   }
	
	}
	
}

void swap_close(int sd)
{
	if (session_id == 0 || sd != session_id)
		return;

	else
		session_id = 0;

	swap_disconnect(sd);	// in sdp.o
}
