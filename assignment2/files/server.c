/**
 * Skeleton file for server.c
 * 
 * You are free to modify this file to implement the server specifications
 * as detailed in Assignment 3 handout.
 * 
 * As a matter of good programming habit, you should break up your imple-
 * mentation into functions. All these functions should contained in this
 * file as you are only allowed to submit this file.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
// Include necessary header files

/**
 * The main function should be able to accept a command-line argument
 * argv[0]: program name
 * argv[1]: port number
 * 
 * Read the assignment handout for more details about the server program
 * design specifications.
 */ 

int port;
int socket_fd;
struct sockaddr_in socket_addr;

int main(int argc, char *argv[])
{
	//return error if port argument is not specified
	if(argc <= 1){
		return -1;
	}
	port = atoi(argv[1]);
	//return error if port is within privileged port range 0-1023 inclusive
	if(port < 1024 || port > 65535){
		return -1;
	}
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	//return error and exit if socket creation returns error
	if(socket_fd == -1){
		printf("Error creating socket\n");
		return -1;
	}

	struct sockaddr_in socket_addr = {AF_INET, htons(port), INADDR_ANY}; //socket address using ipv4, var port, any ipv4 address may connect
	
	if(bind(socket_fd, (struct sockaddr *) &socket_addr, sizeof(socket_addr)) == -1){
		printf("Error binding socket\n");
		return -1;
	}
	printf("%d\n", port);
	return 0;
}

