// server.c tcp threaded
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <pthread.h> // for threading
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// gcc -o server-threaded server-threaded.c -lpthread


// Function designed for chat between client and server.
void *func(void *arg)
{
    int connfd = *((int *)arg);
    char buff[MAX];
    int n;
    
    // Infinite loop for chat
    for (;;) {
        bzero(buff, MAX);

        // Read the message from client and copy it into the buffer
        read(connfd, buff, sizeof(buff));
        
        // Print buffer which contains the client contents
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        
        // Copy server message into the buffer
        while ((buff[n++] = getchar()) != '\n');
        
        // Send the buffer to client
        write(connfd, buff, sizeof(buff));

        // If the message contains "exit", then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }

    // Close the connection
    close(connfd);
    return NULL;
}

// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // Socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    len = sizeof(cli);

    // Infinite loop to keep accepting clients
    while (1) {
        // Accept the data packet from client and verification
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("Server accept failed...\n");
            exit(0);
        }
        else
            printf("Server accepted the client...\n");

        // Create a new thread for each client connection
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, func, (void *)&connfd) != 0) {
            printf("Failed to create thread\n");
        }
    }

    // After chatting close the socket
    close(sockfd);
}
