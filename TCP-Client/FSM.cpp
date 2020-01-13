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
#include <fstream>
#include <iostream>
#include "ack_packet.h"
#include "packet.h"

using namespace std;

int FSM::execute(){
    if(this->state == "handShake"){
        return this->listen_state_action();
    } else if(this->state == "receive"){
        return this->receive_state_action();
    }
//    else if(this->state == "waitForAcks"){
//        return this->wait_for_acks_action();
//    }
}

int FSM::listen_state_action() {
    ack_packet ack;
    ack.ackno = 365434;
    this->set_timeout();
    int state;
    state = recvfrom(this->socketFileDescriptor, &ack, sizeof(ack), 0, this->serverAddrInfo->ai_addr, reinterpret_cast<socklen_t *>(this->serverAddrInfo->ai_addrlen));
    if(ack.ackno != 0){
        return -1;
    }
    this->state = "receive";
    return ack.len;
}
void FSM::set_timeout(){
    this->tv.tv_sec = 0;
    this->tv.tv_usec = 100;
    if (setsockopt(socketFileDescriptor, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        perror("Error");
    }
}


int FSM::receive_state_action() {
    packet data_packet;
    struct sockaddr_storage their_addr;
    socklen_t addr_len = sizeof their_addr;
    int recv = recvfrom(this->socketFileDescriptor, &data_packet, sizeof(data_packet), 0,
                        (struct sockaddr *)&their_addr, &addr_len);
    this->client_addr = their_addr;
    this->addr_len = addr_len;
    if(data_packet.seqno == ackNum) {
        outfile->write(data_packet.data, data_packet.len - 6);
        ackNum += data_packet.len - 6;
        this->ackPacket.ackno = ackNum;
        int x = this->send_ack();
    } else if(data_packet.seqno > ackNum){
        int x = this->send_ack();
    } else{
        this->ackPacket.ackno = data_packet.seqno + MAXSIZE;
        int x = this->send_ack();
    }
    return data_packet.moreFragments;
}

int FSM::send_ack(){
    float r = (rand() % 100);
    r = r/100.0;
    cout << r << endl;
//    if(r > 0.01){
        cout << "send ack " << this->ackPacket.ackno << endl;
        int bytes;
        if((bytes =  sendto(this->socketFileDescriptor, &(this->ackPacket), sizeof(this->ackPacket),
                0, (struct sockaddr *)&this->client_addr, this->addr_len)) == -1){
            perror("talker: sendto");
            exit(1);
        }
        return bytes;
//    }

}