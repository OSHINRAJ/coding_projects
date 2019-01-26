/*
 * @file	:	client.c
 * @brief	:	This file contains the source code for the client side of file
 *				transfer using Socket programing. The client sends the name of
 *				the file to be transfered to the server and recieves the file
 *				from the server if the file is valid, else prints "No such
 *				file".
 * @author	:	Oshinraj K M
 */

#include "server_client.h"

#define IP_ADDR "127.0.0.1"

int main(void)
{
    int sock_des, c;
    struct sockaddr_in server;
    char filename[20], server_reply[MAX_FILE_SIZE] = {'\0'};
    FILE *fp;
    //Create socket
    sock_des = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_des == -1) {
		printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr(IP_ADDR);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    //Connect to remote server
    if (connect(sock_des, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("connect failed. Error");
		return 1;
	}
	puts("Connected\n");
    printf("Enter File name : ");
    scanf("%s", filename);
    fp = fopen(filename, "w");
    if (send(sock_des, filename, strlen(filename), 0) < 0) {
		puts("Send failed");
		return 1;
    }
    //Receive a reply from the server
    if (recv(sock_des, server_reply, MAX_FILE_SIZE, 0) < 0) {
		puts("recv failed");
		return 0;
	}
    if (strcmp(server_reply, no_file) != 0) {
		c = server_reply[0];
		for (int i = 1 ; i < MAX_FILE_SIZE  && c != EOF ; i++) {
			fputc(c, fp);
			c = server_reply[i];
		}
	} else {
		printf("%s", no_file);
    }
    fclose(fp);
	close(sock_des);
	return 0;
}
