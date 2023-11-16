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

/**
 * @brief Utility function used to print an error message to `stderr`.
 * It prints `str` and an implementation-defined error
 * message corresponding to the global variable `errno`.
 * @returns void
 */
void error()
{
    perror("Socket Creation Failed");
    exit(EXIT_FAILURE);
}

/**
 * @brief Main function
 * @returns 0 on exit
 */
int main()
{
    uint32_t sockfd,
        conn;  ///< socket descriptors - Like file handles but for sockets
    char recvbuff[1024],
        sendbuff[1024];  ///< character arrays to read and store string data
                         /// for communication

    struct sockaddr_in server_addr,
        client_addr;  ///< basic structures for all syscalls and functions that
                      /// deal with internet addresses. Structures for handling
                      /// internet addresses
    socklen_t ClientLen;  /// size of address

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error();  ///< Error if the socket descriptor has a value lower than 0 -
                  /// socket wasnt created
    }

    
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Server is running...\n");

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        error();  /// If binding is unsuccessful
    }
   
    listen(sockfd, 5);

    printf("Server is listening...\n");

    /**
     * When a connection is found, a socket is created and connection is
     * accepted and established through the socket descriptor
     */
    conn = accept(sockfd, (struct sockaddr *)NULL, NULL);

    printf("Server is connected...\n");
    pid_t pid;

    #ifdef _WIN32
    #ifdef FORK_WINDOWS
    pid = fork();
    #endif
    #else
    pid = fork();
    #endif

    if (pid == 0)  /// Value of 0 is for child process
    {
        while (1)
        {
            bzero(&recvbuff, sizeof(recvbuff));
            recv(conn, recvbuff, sizeof(recvbuff), 0);
            printf("\nCLIENT : %s\n", recvbuff);
            sleep(5);
            // break;
        }
    }
    else  /// Parent process
    {
        while (1)
        {
            bzero(&sendbuff, sizeof(sendbuff));
            printf("\nType message here: ");
            fgets(sendbuff, 1024, stdin);
            send(conn, sendbuff, strlen(sendbuff) + 1, 0);
            printf("\nMessage Sent!\n");
            sleep(5);
            // break;
        }
    }

    /// Close socket
    close(sockfd);
    printf("Server is offline...\n");
    return 0;
}
