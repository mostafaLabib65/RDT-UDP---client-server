//
// Created by mostafa on 26/11/2019.
//

#ifndef TCP_SERVER_FSM_H
#define TCP_SERVER_FSM_H

#include <string>
#include <asm/socket.h>
#include <sys/socket.h>
#include <vector>
#include "ack_packet.h"
#include "packet.h"

using namespace std;

class FSM {
private:
    string state;
    struct sockaddr *clientAddrInfo;
    socklen_t clientAddrLen;
    int socketFileDescriptor;
    float prob;
    struct timeval tv;
    int send_state_action();
    int listen_state_action();
    int wait_for_acks_action();
    int send_ack();
    void set_timeout();


public:
    struct ack_packet ackPacket;
    FSM(string state, int socketFileDescriptor, sockaddr *client, socklen_t clientAddrLen, int seed, float prob){
        this->clientAddrInfo = client;
        this->state = state;
        this->socketFileDescriptor = socketFileDescriptor;
        this->clientAddrLen = clientAddrLen;
        this->prob = prob;
        ackPacket.ackno = 0;
        ackPacket.len = 4;
        srand(seed);
    }
    vector<packet> data;
    int execute();
    string get_state();
    void force_state(string new_state);
};


#endif //TCP_SERVER_FSM_H
