//
// Created by mostafa on 02/12/2019.
//

#ifndef TCP_SERVER_UDP_SOCKET_H
#define TCP_SERVER_UDP_SOCKET_H

#define MYPORT "4950"
#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

class UDP_Socket {
private:
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
public:
    UDP_Socket(const char * client_port, const char *client_ip){
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        if ((rv = getaddrinfo(client_ip, client_port, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            exit(1);
        }
        // loop through all the results and make a socket
        for(p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype,
                                 p->ai_protocol)) == -1) {
                perror("talker: socket");
                continue;
            }
            break;
        }
        if (p == NULL) {
            fprintf(stderr, "talker: failed to bind socket\n");
            exit(2);
        }

    }
    int get_udp_socket(){
        return this->sockfd;
    }

    addrinfo* get_client_info(){
        return this->p;
    }
};


#endif //TCP_SERVER_UDP_SOCKET_H
