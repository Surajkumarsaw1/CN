// udp_server.c
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
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is listening on port %d\n", PORT);

    while (1) {
        // Receive message from client
        memset(buffer, 0, BUFFER_SIZE);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL,
                         (struct sockaddr *)&client_addr, &addr_len);
        buffer[n] = '\0'; // Null-terminate the received string

        printf("Client: %s\n", buffer);

        // Reverse the received string
        reverseString(buffer);

        // Send the reversed string back to the client
        sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM,
               (const struct sockaddr *)&client_addr, addr_len);
        
        printf("Reversed string sent to client: %s\n", buffer);
    }

    close(sockfd);
    return EXIT_SUCCESS;
}