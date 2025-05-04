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

char* get(FILE *in_file){
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
	char in_fp[] = "sample1.txt";
	FILE *in_file;
	in_file = fopen("sample6.txt", "r");
	printf("%s", get(in_file));
	free(sending_buffer_ptr);
	return 0;
}
