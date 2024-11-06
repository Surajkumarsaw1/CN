// client.c tcp simultaneously
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8792
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Could not create socket");
        return EXIT_FAILURE;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connect failed");
        return EXIT_FAILURE;
    }

    puts("Connected to the server");

    // Communication loop
    while (1) {
        printf("Write your message: ");
        fgets(message, BUFFER_SIZE, stdin); // Use fgets instead of gets for safety

        // Remove newline character from the end of the string
        message[strcspn(message, "\n")] = '\0';

        // Send some data
        if (send(sock, message, strlen(message), 0) < 0) {
            puts("Send failed");
            break;
        }

        // Receive a reply from the server
        memset(server_reply, 0, BUFFER_SIZE);
        if (recv(sock, server_reply, BUFFER_SIZE, 0) < 0) {
            puts("Recv failed");
            break;
        }

        printf("Server reply: %s\n", server_reply);

        // Exit condition
        if (strcmp(message, "bye") == 0) {
            break; // Exit loop if "bye" is sent
        }
    }

    close(sock);
    return EXIT_SUCCESS;
}