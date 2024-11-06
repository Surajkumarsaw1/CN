// server.c tcp simultaneously
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8792
#define BUFFER_SIZE 1024

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    int read_size;

    while ((read_size = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_size] = '\0'; // Null-terminate the received string
        printf("Client: %s\n", buffer);

        // Respond based on the client's message
        if (strcmp(buffer, "hi") == 0) {
            send(client_sock, "wellcome", strlen("wellcome"), 0);
        } else if (strcmp(buffer, "bye") == 0) {
            send(client_sock, "see you", strlen("see you"), 0);
        } else {
            send(client_sock, buffer, read_size, 0); // Echo back the message
        }
    }

    if (read_size == 0) {
        puts("Client disconnected");
    } else if (read_size == -1) {
        perror("recv failed");
    }

    close(client_sock);
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server, client;
    socklen_t addr_len = sizeof(client);

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
        handle_client(client_sock); // Handle client in a separate function
    }

    if (client_sock < 0) {
        perror("Accept failed");
        return EXIT_FAILURE;
    }

    close(server_fd);
    return EXIT_SUCCESS;
}