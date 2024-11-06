// server.c TCP file server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 8080
#define IP_ADDRESS "127.0.0.1"

void sendFile(int newSocket) {
    char *filename = "file_to_send.txt"; // File to be sent
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("File not found");
        return;
    }

    char buffer[BUFFER_SIZE];
    while (fread(buffer, sizeof(char), BUFFER_SIZE, fp) > 0) {
        send(newSocket, buffer, sizeof(buffer), 0);
        memset(buffer, 0, BUFFER_SIZE);
    }
    fclose(fp);
}

int main() {
    int server_fd, newSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int port = DEFAULT_PORT;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to the port
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    address.sin_port = htons(port);

    // Binding the socket
    while (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed. Please enter a different port number: ");
        printf("Enter port number to bind: ");
        scanf("%d", &port);
        address.sin_port = htons(port); // Update port in address structure
    }

    // Listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d\n", IP_ADDRESS, port);

    // Accepting a connection
    if ((newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    sendFile(newSocket);
    
    printf("File sent successfully\n");
    
    close(newSocket);
    close(server_fd);
    
    return 0;
}