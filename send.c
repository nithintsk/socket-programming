/************************************************************************
 * * Sender for multicast traffic                                       *
 * *                                                                    *
 * * Author:  Nithin Senthil Kumar                                      *
 * *                                                                    *
 * * Usage:   ./client                                                  *
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 1820
// The range 239.0.0.0 to 239.255.255.255 can be used locally for multicast
#define GROUP "239.0.0.1"

int main(int argc, char* argv[])
{
	int sd, rc, i;
	struct sockaddr_in mc_addr;
	socklen_t addr_len;
    unsigned char message[12] = "Hello World";
    
	// Set up the socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("Error while setting up socket");
        exit(1);
    }

    // Set up the multicast address
    mc_addr.sin_family = AF_INET;
    mc_addr.sin_addr.s_addr = inet_addr(GROUP);
    mc_addr.sin_port = htons(PORT);
    addr_len = sizeof(mc_addr);

    // Send the message every 5 seconds for 5 times
    for ( i = 0; i < 10; i++) {
        printf("Sending Message.\n");
        rc = sendto(sd, message, sizeof(message), 0,
                (struct sockaddr *) &mc_addr, addr_len);
        if (rc < 0) {
            perror("Error while sending");
            exit(1);
        }
        sleep(2);
    }
}
