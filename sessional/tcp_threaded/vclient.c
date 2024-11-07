// client.c tcp threaded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <arpa/inet.h> // inet_addr()
#include <netinet/in.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function to try connecting to a specific IP address
int try_connect(const char *ip_addr)
{
    int sockfd;
    struct sockaddr_in servaddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(ip_addr); // Convert IP address to binary form

    // Try to connect
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) == 0) {
        return sockfd; // Connection successful
    } else {
        close(sockfd);
        return -1; // Connection failed
    }
}

// Function for communication with the server
void communicate(int sockfd)
{
    char buff[MAX];
    int n;

    for (;;) {
        memset(buff, 0, sizeof(buff));

        // Read input from user
        printf("Enter the string: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n' && n < MAX - 1)
            ;
        buff[n - 1] = '\0'; // Null-terminate the buffer

        // Send the buffer to the server
        write(sockfd, buff, strlen(buff) + 1);

        // Read the server response
        memset(buff, 0, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server: %s\n", buff);

        // If the message contains "exit", then client exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }

    // Close the socket
    close(sockfd);
}

int main()
{
    int sockfd;
    char ip_addr[16];
    int connected = 0;

    // Try to connect to 127.0.0.1 first
    printf("Trying IP: 127.0.0.1\n");
    sockfd = try_connect("127.0.0.1");

    if (sockfd == -1) {
        // Prompt for IP address if connection fails
        printf("Failed to connect to 127.0.0.1\n");
        printf("Enter IP address to connect to: ");
        scanf("%15s", ip_addr); // Read IP address from user

        // Try to connect to the user-specified IP address
        sockfd = try_connect(ip_addr);

        if (sockfd != -1) {
            printf("Connected to server at %s\n", ip_addr);
            connected = 1;
        } else {
            printf("Failed to connect to %s\n", ip_addr);
        }
    } else {
        printf("Connected to server at 127.0.0.1\n");
        connected = 1;
    }

    if (connected) {
        // Communicate with the server
        communicate(sockfd);
    } else {
        printf("Failed to connect to any server.\n");
    }

    return 0;
}

