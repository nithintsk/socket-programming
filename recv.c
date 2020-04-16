#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 1820
#define GROUP "239.0.0.1"

int main() {
	int sd, rc;
	struct sockaddr_in mc_addr;
	struct ip_mreq mc_req;
    char message[12];
	socklen_t mc_len;

    // Set up the socket
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
        perror("Error while setting up socket");
        exit(1);
    }
    
    // Set up the server address
    bzero(&mc_addr, sizeof(struct sockaddr_in));
    mc_addr.sin_family = AF_INET;
	mc_addr.sin_port = htons(PORT);
	mc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    mc_len = sizeof(mc_addr);

    // Bind to an ephemeral port
	if (bind(sd, (struct sockaddr *) &mc_addr, mc_len) < 0) {
        perror("Error while binding socket");
        exit(1);
    }

    // Add multicast membership to socket options
	bzero(&mc_req, sizeof(struct ip_mreq));
    mc_req.imr_multiaddr.s_addr = inet_addr(GROUP);
    mc_req.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                &mc_req, sizeof(struct ip_mreq)) < 0) {
        perror("Error while adding membership");
        exit(1);
    }

    // Receive the messages
    while (1) {
        rc = recvfrom(sd, message, sizeof(message), 0, 
                (struct sockaddr *) &mc_addr, &mc_len);
        if (rc < 0) {
            perror("Error while receiving");
            exit(1);
        }

        printf("Received message: %s\n", message);
    }
}
