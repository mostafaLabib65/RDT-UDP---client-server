//
// Created by mostafa on 28/11/2019.
//

#ifndef TCP_CLIENT_CLIENT_H
#define TCP_CLIENT_CLIENT_H


#include <netdb.h>
#include "FSM.h"

class Client {
private:
    int socketFileDescriptor;
    struct addrinfo *serverAddrInfo;
    string fileName;
    std::ofstream outfile;
    FSM *fsm;
public:
    Client(int socketFileDescriptor, addrinfo *serverAddrInfo, string fileName){
        this->socketFileDescriptor = socketFileDescriptor;
        this->serverAddrInfo = serverAddrInfo;
        this->fileName = fileName;
        this->outfile.open(fileName, std::ofstream::binary);
        this->fsm = new FSM("handShake", socketFileDescriptor, serverAddrInfo, serverAddrInfo->ai_addrlen, &this->outfile);
    }
    void rdt();

};


#endif //TCP_CLIENT_CLIENT_H
