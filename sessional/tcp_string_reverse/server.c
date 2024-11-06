// server.c tcp reverse
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8792
#define BUFFER_SIZE 1024

void reverseString(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server, client;
    socklen_t addr_len = sizeof(client);
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Could not create socket");
        return EXIT_FAILURE;
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    listen(server_fd, 3);
    puts("Waiting for incoming connections...");

    // Accept and incoming connection
    while ((client_sock = accept(server_fd, (struct sockaddr *)&client, &addr_len))) {
        puts("Connection accepted");

        // Receive a message from the client
        memset(buffer, 0, BUFFER_SIZE);
        int read_size = recv(client_sock, buffer, sizeof(buffer), 0);
        if (read_size > 0) {
            buffer[read_size] = '\0'; // Null-terminate the received string
            printf("Client: %s\n", buffer);

            // Reverse the received string
            reverseString(buffer);

            // Send the reversed string back to the client
            send(client_sock, buffer, strlen(buffer), 0);
            printf("Reversed string sent to client: %s\n", buffer);
        }

        close(client_sock);
        puts("Client disconnected");
    }

    if (client_sock < 0) {
        perror("Accept failed");
        return EXIT_FAILURE;
    }

    close(server_fd);
    return EXIT_SUCCESS;
}