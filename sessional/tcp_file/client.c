// client.c TCP file server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 8080
#define IP_ADDRESS "127.0.0.1"

void receiveFile(int sockfd) {
    char *filename = "received_file.txt"; // File to save received data
    FILE *fp = fopen(filename, "wb");
    
    if (fp == NULL) {
        perror("File creation failed");
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytesReceived;

    while ((bytesReceived = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), bytesReceived, fp);
        memset(buffer, 0, BUFFER_SIZE);
    }
    
    fclose(fp);
}

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;

    // Creating socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DEFAULT_PORT);

   // Convert IPv4 and IPv6 addresses from text to binary form
   if(inet_pton(AF_INET, IP_ADDRESS, &serv_addr.sin_addr) <= 0) {
       printf("\nInvalid address/ Address not supported \n");
       return -1;
   }

   // Connecting to server
   if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
       printf("\nConnection Failed \n");
       return -1;
   }

   receiveFile(sockfd);
   
   printf("File received successfully\n");
   
   close(sockfd);
   
   return 0;
}