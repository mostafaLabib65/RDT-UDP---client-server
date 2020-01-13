//
// Created by mostafa on 26/11/2019.
//

#ifndef TCP_SERVER_SERVER_H
#define TCP_SERVER_SERVER_H


#include "FSM.h"
#include "CongestionControlFSM.h"

class Server {
private:
    FSM *fsm;
    string fileName;
    int socketFileDescriptor;
    string get_data(string path);
    vector<packet> prepare_packet(string *fileData, int seqNo, int windowSize);
    CongestionControlFSM *congestionFlowControlFsm;

public:
    Server(int socketFileDescriptor, sockaddr *clientAddrInfo, socklen_t clientAddrLen, string fileName, int seed, float prob){
        this->socketFileDescriptor = socketFileDescriptor;
        this->fileName = fileName;
        this->fsm = new FSM("listen", socketFileDescriptor, clientAddrInfo, clientAddrLen, seed, prob);
        this->congestionFlowControlFsm = new CongestionControlFSM();
    }
    void rdt();
};


#endif //TCP_SERVER_SERVER_H
