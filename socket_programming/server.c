/*
 * @file	:	server.c
 * @brief	:	This file contains the source code for the server side of
 *				file transfer using Socket programing.The server takes the file
 *				name from the client and if the file is present in server the
 *				file is trensfered else prints "No Such file"
 * @author	:	Oshinraj K M
 */
#include "server_client.h"

int main(void)
{
    int socket_desc, client_sock, c;
	int port;
    struct sockaddr_in server, client;
    char client_message[MAX_FILE_SIZE] = {'\0'};
    FILE *fp;
    
	//Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);
	
	//Bind
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		//print the error message
		perror("bind failed. Error");
		return 1;
    }
    puts("bind done");
    
	//Listen
    listen(socket_desc, 3);
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
	//accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
    if (client_sock < 0) {
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");
	
	//Receive a message from client
	if (recv(client_sock, client_message, MAX_FILE_SIZE, 0) < 0) {
		perror("receive failed");
		return 1;
    }
    fp = fopen(client_message, "r");
    if (fp == NULL) {
		strcpy(client_message, no_file);
    } else {
		for (int i = 0 ; i < MAX_FILE_SIZE &&  c != EOF ; i++) {
			c = fgetc(fp);
			client_message[i] = c;
		}
	}
	//Send the message back to client
	write (client_sock, client_message, strlen(client_message));
	fclose(fp);
    close(socket_desc);
    return 0;
}

