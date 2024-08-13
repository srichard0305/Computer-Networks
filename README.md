# Computer-Networks

## Project Description
Part 1 of the project uses UDP to create a connection from a server program to a client program. The client program sends the IP, Port, and Message to the sever program through the command line.
The server receives the message and echos it back to the client. Both programs display the message on screen.

Part 2 of the project uses TCP to create a connection from a server program to a client program. The client program sends the IP, Port, and Message to the sever program through the command line.
The server receives the message and echos it back to the client. Both programs display the message on screen.

Part 3 of the project uses the Stop and Wait protocol that will create a connection to from a server program to a client program. The client program will send a frame containing the sequence number,
check sum, and the message. The client program will start a timer and wait for the ACK message from the server. The server will receive the frame and check the sequence number and checksum. If the sequence number is incorrect the server will send an ACK message back containing the correct sequence number.  If the checksum is incorrect the server will discard the packet and the timer will run out. The client program will resend the packet once the timer runs out. 

Part 4 of the project uses the same Stop and Wait protocol as part 3. This program will read in a text file and send it to the server program. The server program will read the frame sent from the client and output
its contents to an output text file. 

## Install
All programs were compliled on Linux OS using gcc, for example, such as: gcc tcp_echo_server.c -o 
