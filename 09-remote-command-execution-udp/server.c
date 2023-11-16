
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
    char recv_msg[1024],
        success_message[] =
            "Command Executed Successfully!\n";
    struct sockaddr_in server_addr,
        client_addr;
    socklen_t clientLength = sizeof(client_addr);  /// size of address
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        error();
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        error();  /// If binding is unsuccessful
    }

    printf("Server is Connected Successfully...\n");

    while (1)
    {
        bzero(recv_msg, sizeof(recv_msg));
        recvfrom(sockfd, recv_msg, sizeof(recv_msg), 0,
                 (struct sockaddr *)&client_addr, &clientLength);
        printf("Command Output: \n");
        system(recv_msg);
        printf("Command Executed\n");
        sendto(sockfd, success_message, sizeof(success_message), 0,
               (struct sockaddr *)&client_addr, clientLength);
    }

    /// Close socket
    close(sockfd);
    printf("Server is offline...\n");
    return 0;
}
