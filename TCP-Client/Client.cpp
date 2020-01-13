//
// Created by mostafa on 28/11/2019.
//

#include <zconf.h>
#include <iostream>
#include <cstring>
#include "Client.h"
void Client::rdt() {
    int state = this->fsm->execute(); // Waiting For server ack
    while(state == -1){
        int numbytes;
        if ((numbytes = sendto(this->socketFileDescriptor, this->fileName.c_str(), this->fileName.length(), 0,
                               this->serverAddrInfo->ai_addr, this->serverAddrInfo->ai_addrlen)) == -1) {
            perror("talker: sendto");
            exit(1);
        }
        state = this->fsm->execute();
    }
    this->fsm->tv.tv_sec = 0;
    this->fsm->tv.tv_usec = 0;
    if (setsockopt(this->socketFileDescriptor, SOL_SOCKET, SO_RCVTIMEO,&this->fsm->tv,sizeof(this->fsm->tv)) < 0) {
        perror("Error");
    }
    bool moreFragments = 1;
    while(moreFragments){
        moreFragments = this->fsm->execute();
    }
    cout << moreFragments<<endl;
    outfile.close();
}