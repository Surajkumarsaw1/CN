// udp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8792
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Communication loop
    while (1) {
        printf("Enter a string to send to the server (or 'exit' to quit): ");
        fgets(message, BUFFER_SIZE, stdin); // Use fgets instead of gets for safety

        // Remove newline character from the end of the string
        message[strcspn(message, "\n")] = '\0';

        // Exit condition
        if (strcmp(message, "exit") == 0) {
            break; // Exit loop if "exit" is entered
        }

        // Send message to the server
        sendto(sockfd, message, strlen(message), MSG_CONFIRM,
               (const struct sockaddr *)&server_addr, sizeof(server_addr));

        // Receive a reply from the server
        socklen_t addr_len = sizeof(server_addr);
        memset(server_reply, 0, BUFFER_SIZE);
        
        recvfrom(sockfd, server_reply, BUFFER_SIZE, MSG_WAITALL,
                 (struct sockaddr *)&server_addr, &addr_len);

        printf("Server reply: %s\n", server_reply);
    }

    close(sockfd);
    return EXIT_SUCCESS;
}