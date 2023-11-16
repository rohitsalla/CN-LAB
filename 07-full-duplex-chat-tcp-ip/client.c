
#ifdef _WIN32
#define bzero(b, len) \
    (memset((b), '\0', (len)), (void)0) /**< BSD name not in windows */
#define pid_t int
#define close _close
#include <Ws2tcpip.h>
#include <io.h>
#include <windows.h>
#include <winsock2.h>
#include "fork.h"
#define sleep(a) Sleep(a * 1000)
#else
#include <arpa/inet.h>  /// For the type in_addr_t and in_port_t
#include <netdb.h>  /// For structures returned by the network database library - formatted internet addresses and port numbers
#include <netinet/in.h>  /// For in_addr and sockaddr_in structures
#include <sys/socket.h>  /// For macro definitions related to the creation of sockets
#include <sys/types.h>  /// For definitions to allow for the porting of BSD programs
#include <unistd.h>
#endif
#include <stdint.h>  /// For specific bit size values of variables
#include <stdio.h>  /// Variable types, several macros, and various functions for performing input and output
#include <stdlib.h>  /// Variable types, several macros, and various functions for performing general functions
#include <string.h>  /// Various functions for manipulating arrays of characters

#define PORT 10000  /// Define port over which communication will take place

void error()
{
    perror("Socket Creation Failed");
    exit(EXIT_FAILURE);
}
int main()
{
    /** Variable Declarations */
    uint32_t
        sockfd;  
    char sendbuff[1024],
        recvbuff[1024];  ///< character arrays to read and store string data
                         /// for communication

    struct sockaddr_in
        server_addr;  
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error();
    }

  
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Client is running...\n");

    /**
     * Connects the client to the server address using the socket descriptor
     * This enables the two to communicate and exchange data
     */
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Client is connected...\n");
    pid_t pid;
    pid = fork();

    if (pid == 0)  /// Value of 0 is for child process
    {
        while (1)
        {
            bzero(&sendbuff, sizeof(sendbuff));
            printf("\nType message here: ");
            fgets(sendbuff, 1024, stdin);
            send(sockfd, sendbuff, strlen(sendbuff) + 1, 0);
            printf("\nMessage sent!\n");
            sleep(5);
            // break;
        }
    }
    else  /// Parent Process
    {
        while (1)
        {
            bzero(&recvbuff, sizeof(recvbuff));
            recv(sockfd, recvbuff, sizeof(recvbuff), 0);
            printf("\nSERVER: %s\n", recvbuff);
            sleep(5);
            // break;
        }
    }

    /// Close Socket
    close(sockfd);
    printf("Client is offline...\n");
    return 0;
}
