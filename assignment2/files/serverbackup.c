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
char *sending_buffer_ptr;

char* get(char* client_command){
	char get[] = "GET\0";
	char filename[100];
	sscanf(client_command, "%s %s", filename, filename);
	//check for arguments, if none exist then return 500 Get Error
	if(strcasecmp(get,filename) == 0){
		return "SERVER 500 Get Error\n";
	}
	FILE *in_file = fopen(filename, "r");
	if(in_file == NULL){
		return "SERVER 404 Not Found\n";
	}
	/*
	static char file_contents[500];
	char c;
	for(int i = 0; c != EOF; i++){
		c = getc(in_file);
		file_contents[i] = c;
	}
	fclose(in_file);
	return file_contents;
	*/
	char c;
	int char_count = 0;
	for(c = getc(in_file); c != EOF; c = getc(in_file)){
		char_count++;
	}
	sending_buffer_ptr = (char *) malloc(sizeof(char) * char_count);
	if(sending_buffer_ptr == NULL){
		printf("Error allocating memory");
		return NULL;
	}
	rewind(in_file);
	c = '\0';
	for(int i = 0; c != EOF; i++){
		c = getc(in_file);
		sending_buffer_ptr[i] = c;
	}
	fclose(in_file);
	return sending_buffer_ptr;
}

int main(int argc, char *argv[])
{
	int port; //port to listen on
	int socket_fd; //socket file descriptor
	int client_fd; //client fild descriptor

	//return error if port argument is not specified
	if(argc <= 1){
		return -1;
	}
	port = atoi(argv[1]);
	//return error if port is within privileged port range 0-1023 inclusive
	if(port < 1024 || port > 65535){
		return -1;
	}
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
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
	listen: printf("Listening for connections\n");
	if(listen(socket_fd, SOMAXCONN) < 0){
		printf("Error listening");
		return -1;
	}
	client_fd = accept(socket_fd, (struct sockaddr*) &socket_addr, (socklen_t*)&sock_addr_len);
	if(client_fd < 0){
		printf("Error accepting connection");
		return -1;
	}
	printf("Connection accepted\n");
	int send_to_client = send(client_fd, "HELLO", strlen("HELLO"), 0);
	if(send_to_client < 0){
		printf("Error sending acknowledgement message");
		return -1;
	}
	while(1){
		char receive_buffer[100]; //buffer where received msgs are stored
		ssize_t receive = recv(client_fd, receive_buffer, 100, 0);
		if(receive <= 0){
			printf("Error receiving\n");
			goto listen;
		}
		if(strlen(receive_buffer) != 0){
			char command[100];
			sscanf(receive_buffer, "%s", command);
			if(strcasecmp(command, "BYE") == 0){
				int close(int client_fd);
				printf("Connection closed\n");
				goto listen;
			}
			else if(strcasecmp(command, "GET") == 0){
				char *file_contents = get(receive_buffer);
				int file_contents_length = strlen(file_contents);
				send_to_client = send(client_fd, file_contents, file_contents_length, 0);
				free(sending_buffer_ptr);
			}
			else if(strcasecmp(command, "PUT") == 0){
				//call put()
			}
			else{
				send_to_client = send(client_fd, "SERVER 502 Command Error\n", strlen("SERVER 502 Command Error\n"), 0);
			}
		}
	}
}
