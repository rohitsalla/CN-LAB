#include<sys/types.h>
#include<sys/socket.h>
#include<net/if_arp.h>
#include<sys/ioctl.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<complex.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
        struct sockaddr_in sin={0};
        struct arpreq myarp={{0}};
        unsigned char *ptr;
        int sd;
        sin.sin_family=AF_INET;
        if(inet_aton(argv[1],&sin.sin_addr)==0)
        {
                printf("IP address Entered '%s' is not valid \n",argv[1]);
                exit(0);
        }
        memcpy(&myarp.arp_pa,&sin,sizeof(myarp.arp_pa));
        strcpy(myarp.arp_dev,"echo");
        sd=socket(AF_INET,SOCK_DGRAM,0);
       
       if(ioctl(sd,SIOCGARP,&myarp)==1)
        {
                printf("No Entry in ATP cache for '%s'\n",argv[1]);
                exit(0);
        }
        ptr=&myarp.arp_pa.sa_data[0];
        printf("\nMAC Address for '%s' : ",argv[1]);
        printf("%x:%x:%x:%x:%x:%x\n",*ptr,*(ptr+1),*(ptr+2),*(ptr+3),*(ptr+4),*(ptr+5));
        printf("\n\t\t\t\t%x:%x:%x:%x:%x:%x\n", myarp.arp_ha.sa_data[0],
                    myarp.arp_ha.sa_data[1], myarp.arp_ha.sa_data[2],
                    myarp.arp_ha.sa_data[3], myarp.arp_ha.sa_data[4], myarp.arp_ha.sa_data[5]);
        return 0;
}
