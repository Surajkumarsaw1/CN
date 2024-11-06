// tcp_server.c add
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8792
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Could not create socket");
        return EXIT_FAILURE;
    }

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return EXIT_FAILURE;
    }

    printf("Server is listening on port %d\n", PORT);

    // Accept incoming connection
    while ((client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len))) {
        printf("Connection accepted\n");

        // Receive a message from the client
        memset(buffer, 0, BUFFER_SIZE);
        int read_size = recv(client_sock, buffer, sizeof(buffer), 0);
        if (read_size > 0) {
            buffer[read_size] = '\0'; // Null-terminate the received string
            printf("Received from client: %s\n", buffer);

            // Parse the two numbers from the received message
            int num1, num2;
            sscanf(buffer, "%d %d", &num1, &num2);

            // Calculate the sum
            int sum = num1 + num2;

            // Send the result back to the client
            snprintf(buffer, BUFFER_SIZE, "Sum: %d", sum);
            send(client_sock, buffer, strlen(buffer), 0);
            printf("Sent to client: %s\n", buffer);
        }

        close(client_sock);
        printf("Client disconnected\n");
    }

    close(server_fd);
    return EXIT_SUCCESS;
}