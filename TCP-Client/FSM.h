//
// Created by mostafa on 26/11/2019.
//

#ifndef TCP_SERVER_FSM_H
#define TCP_SERVER_FSM_H

#include <string>
#include <fstream>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <sys/socket.h>
#include "ack_packet.h"
#include "packet.h"

using namespace std;

class FSM {
private:
    string state;
    int ackNum = 0;
    ofstream *outfile;
    struct ack_packet ackPacket;
    struct addrinfo *serverAddrInfo;
    int serverAddrLen;
    int socketFileDescriptor;
    int receive_state_action();
    int listen_state_action();
    int send_ack();
    struct sockaddr_storage client_addr;
    socklen_t addr_len = sizeof client_addr;
    void set_timeout();

public:
    FSM(string state, int socketFileDescriptor, addrinfo *server, int serverAddrLen, ofstream *outfile){
        this->serverAddrInfo = server;
        this->state = state;
        this->socketFileDescriptor = socketFileDescriptor;
        this->serverAddrLen = serverAddrLen;
        this->outfile = outfile;
        srand(time(nullptr));
        ackPacket.ackno = 0;
        ackPacket.len = 4;
    }
    struct timeval tv;
    int execute();
    string get_state();
    void force_state(string new_state);
};


#endif //TCP_SERVER_FSM_H
