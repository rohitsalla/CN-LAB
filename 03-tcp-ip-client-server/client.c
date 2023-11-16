#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to the server");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Send data to the server
    printf("Enter data to send to the server: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
        perror("Error sending data to the server");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Receive data from the server
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
        perror("Error receiving data from the server");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    printf("Received from server: %s\n", buffer);

    // Close the socket
    close(clientSocket);

    return 0;
}
