//
// Created by mostafa on 26/11/2019.
//

#include <cstring>
#include "FSM.h"
#include <sys/socket.h>
#include "string"
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include "ack_packet.h"
#include "packet.h"

using namespace std;

int FSM::execute(){
    if(this->state == "send"){
        return this->send_state_action();
    } else if(this->state == "listen"){
        return this->listen_state_action();
    }
    else if(this->state == "waitForAcks"){
        return this->wait_for_acks_action();
    }
}

int FSM::listen_state_action() {
    if(this->send_ack() == -1){
        perror("talker: sendto");
        exit(1);
    } else{
        this->state = "send";
        return 1;
    }
}

int FSM::send_state_action() {
    this->state = "waitForAcks";
    for(int i = 0; i < data.size(); i++){
        float r = (rand() % 100);
        r = r/100.0;
        if(r > this->prob) {
            int numBytes = sendto(this->socketFileDescriptor, &(data[i]), sizeof(data[i]), 0, clientAddrInfo, (this->clientAddrLen));
//            cout << "sent seqNum " << data[i].seqno<< " : " << data[i].moreFragments << endl;
        }
    }
    return  1;
}

int FSM::send_ack(){
    return sendto(this->socketFileDescriptor, &(this->ackPacket), sizeof(this->ackPacket), 0, clientAddrInfo, (this->clientAddrLen));
}

int FSM::wait_for_acks_action(){
    ack_packet ack;
    int status;
    this->set_timeout();
    status = recvfrom(socketFileDescriptor, &ack, sizeof(ack) , 0, (clientAddrInfo), &clientAddrLen);
    if(status == -1){
//        cout <<"time out\n";
        return status;
    }
//    cout << "recieved ack "<< ack.ackno <<endl;
    return ack.ackno;
}

void FSM::set_timeout(){
    this->tv.tv_sec = 0;
    this->tv.tv_usec = 100;
    if (setsockopt(socketFileDescriptor, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        perror("Error");
    }
}

void FSM::force_state(string new_state) {
    this->state = new_state;
}