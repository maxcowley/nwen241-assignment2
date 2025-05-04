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
#include <string.h>
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
int client_fd;
struct sockaddr_in socket_addr;
char *sending_buffer_ptr;

char* get(FILE *file){
	char c;
	int char_count = 0;
	for(c = getc(file); c != EOF; c = getc(file)){
		char_count++;
	}
	sending_buffer_ptr = (char *) malloc(sizeof(char) * char_count);
	if(sending_buffer_ptr == NULL){
		printf("Error allocating memory");
		return NULL;
	}
	rewind(file);
	c = '\0';
	for(int i = 0; c != EOF; i++){
		c = getc(file);
		sending_buffer_ptr[i] = c;
	}
	fclose(file);
	return sending_buffer_ptr;
}

int parse_command(char* receive_buffer, ssize_t client_fd){
	
	char command[100];
	char filename[100];
	sscanf(receive_buffer, "%s", command);
	if(strcasecmp(command, "BYE") == 0){
		int close(int client_fd);
		printf("Connection closed\n");
	}
	else if(strcasecmp(command, "GET") == 0){
		ssize_t send_to_client;
		//printf("GET command received");
		sscanf(receive_buffer, "%s %s", command, filename);
		FILE *file;
		file = fopen(filename, "r");
		if(file == NULL){
			
		}
		get(file);
		send_to_client = send(client_fd, "SERVER 200 OK\n\n", strlen("SERVER 200 OK\n\n"), 0);
		send_to_client  = send(client_fd, sending_buffer_ptr, strlen(sending_buffer_ptr), 0);
		send_to_client = send(client_fd, "\n\n", strlen("\n\n"), 0);
		if(send_to_client < 0){
			printf("Error sending file contents");
		}
		printf("%s\n", sending_buffer_ptr);
		free(sending_buffer_ptr);
	}
	else if(strcasecmp(command, "PUT") == 0){
		//printf("PUT command received");
	}
	//else{//return 502;} //Command error: command not found
}

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
	int sock_addr_len = sizeof(socket_addr);

	if(bind(socket_fd, (struct sockaddr *) &socket_addr, sizeof(socket_addr)) == -1){
		printf("Error binding socket\n");
		return -1;
	}
	listen:if(listen(socket_fd, SOMAXCONN) < 0){
		printf("Error listening");
		return -1;
	}
	int client_fd = accept(socket_fd, (struct sockaddr*) &socket_addr, (socklen_t*)&sock_addr_len);
	if(client_fd < 0){
		printf("Error accepting connection");
		return -1;
	}
	char ack_msg[] = "HELLO";
	int connection_ack = send(client_fd, ack_msg, strlen(ack_msg), 0);
	if(connection_ack < 0){
		printf("Error sending acknowledgement message");
		return -1;
	}
	
	while(1){
		char receive_buffer[100];
		ssize_t receive = recv(client_fd, receive_buffer, 100, 0);
		if(receive <= 0){
			printf("Error receiving");
			return 0;
		}
		if(parse_command(receive_buffer, client_fd) == 0){
			goto listen;
		}
	}
	return 0;
}

