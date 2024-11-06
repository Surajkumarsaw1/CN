// tcp_client.c add
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8792
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Could not create socket");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        return EXIT_FAILURE;
    }

    printf("Connected to the server\n");

    // Input two numbers from user
    int num1, num2;
    printf("Enter two numbers separated by space: ");
    
    // Read user input and send to server
    scanf("%d %d", &num1, &num2);
    
    snprintf(message, BUFFER_SIZE, "%d %d", num1, num2);

    // Send message to the server
    if (send(sock, message, strlen(message), 0) < 0) {
        puts("Send failed");
        return EXIT_FAILURE;
    }

    // Receive a reply from the server
    memset(server_reply, 0, BUFFER_SIZE);
    
    if (recv(sock, server_reply, BUFFER_SIZE, 0) < 0) {
        puts("Recv failed");
        return EXIT_FAILURE;
    }

    printf("Server reply: %s\n", server_reply);

    close(sock);
    
    return EXIT_SUCCESS;
}