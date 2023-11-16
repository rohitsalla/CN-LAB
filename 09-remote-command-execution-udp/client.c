#ifdef _WIN32
#define bzero(b, len) \
    (memset((b), '\0', (len)), (void)0) /**< BSD name not in windows */
#define close _close
#include <Ws2tcpip.h>
#include <io.h>
#include <winsock2.h>  /// For the type in_addr_t and in_port_t
#else
#include <arpa/inet.h>  /// For the type in_addr_t and in_port_t
#include <netdb.h>  /// For structures returned by the network database library - formatted internet addresses and port numbers
#include <netinet/in.h>  /// For in_addr and sockaddr_in structures
#include <sys/socket.h>  /// For macro definitions related to the creation of sockets
#include <sys/types.h>  /// For definitions to allow for the porting of BSD programs
#include <unistd.h>
#endif
#include <errno.h>   /// To indicate what went wrong if an error occurs
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
        sockfd;  ///< socket descriptors - Like file handles but for sockets
    char send_msg[1024],
        recv_msg[1024];  ///< character arrays to read and store string data
                         /// for communication

    struct sockaddr_in
        server_addr;  ///< basic structures for all syscalls and functions that
                      /// deal with internet addresses. Structures for handling
                      /// internet addresses
    socklen_t serverLength = sizeof(server_addr);  ///< length of socket

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        error();
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Client is running...\n");
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Client is Connected Successfully...\n");
    while (1)
    {
        printf("\nEnter Command To Be Executed Remotely: \n");
        fgets(send_msg, sizeof(send_msg), stdin);
        sendto(sockfd, send_msg, sizeof(send_msg), 0,
               (struct sockaddr *)&server_addr, serverLength);
        recvfrom(sockfd, recv_msg, sizeof(recv_msg), 0,
                 (struct sockaddr *)&server_addr, &serverLength);
        printf("Server Reply: %s\n", recv_msg);
    }

    /// Close Socket
    close(sockfd);
    printf("Client is offline...\n");
    return 0;
}
