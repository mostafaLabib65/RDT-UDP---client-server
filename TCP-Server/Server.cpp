//
// Created by mostafa on 26/11/2019.
//

#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
#include <vector>
#include "Server.h"
#define MAXSIZE 500
void Server::rdt(){
    string fileData = this->get_data(this->fileName);
    this->fsm->execute(); // send the ack
    int seqNum = 0;
    int prevACK = -1;
    int timeoutCounter = 0;
    int i = 1;
    while (seqNum < fileData.length()){
        float uploading = (float(seqNum)/float(fileData.length()));
        cout << i++ <<  ","<<congestionFlowControlFsm->get_cwnd() << "," <<congestionFlowControlFsm->get_threshold() << endl;
        vector<packet> packets = this->prepare_packet(&fileData, seqNum, congestionFlowControlFsm->get_cwnd());
        int x = packets.size();
        this->fsm->data = packets;
        int sentBytes = this->fsm->execute();
        if(sentBytes > 0){
            int old_cwnd = congestionFlowControlFsm->get_cwnd();
            int x = packets.size();
            for(int i = 0; i < packets.size(); i++){
                int ack = this->fsm->execute();
                if(ack == -1){
                    congestionFlowControlFsm->update_FSM(TIMEOUT);
                    if(timeoutCounter == 10){
                        exit(0);
                    }
                } else if(ack == prevACK){
                    timeoutCounter = 0;
                    congestionFlowControlFsm->update_FSM(DUPACK);
                } else{
                    timeoutCounter = 0;
                    congestionFlowControlFsm->update_FSM(NEWACK);
                    prevACK = ack;
                }
                if(ack != -1 && seqNum == (ack - (packets[i].len - 6))){
                    seqNum = ack;
                } else if(ack == -1){
                    timeoutCounter++;
                    break;
                }
            }
            this->fsm->force_state("send");
        } else if(sentBytes == -1){
            cout << "error\n";
            exit(1);
        } else{
            cout << "client not listening\n";
            exit(1);
        }
    }
}

string Server::get_data(string path){
    ifstream file(path, std::ios::binary | std::ios::in);
    string content;
    if(file) {
        ostringstream ss;
        ss << file.rdbuf();
        content = ss.str();
    }
    return content;
}

vector<packet> Server::prepare_packet(string *fileData, int seqNo, int window_size){
    vector<packet> packets;
    for(int i = 0; i < window_size; i++){
        packet packet{};
        packet.seqno = seqNo;
        if(seqNo + MAXSIZE > fileData->length()){
            packet.len = fileData->length() - seqNo + 6;
            string subString = fileData->substr(seqNo);
            memcpy(packet.data, subString.data(), subString.length());
            packet.moreFragments = false;
            packets.push_back(packet);
            return packets;
        } else{
            packet.len = MAXSIZE + 6;
            string subString = fileData->substr(seqNo, MAXSIZE);
            memcpy(packet.data, subString.data(), MAXSIZE);
            packet.moreFragments = true;
            packets.push_back(packet);
            seqNo += MAXSIZE;
        }
    }
    return packets;
}