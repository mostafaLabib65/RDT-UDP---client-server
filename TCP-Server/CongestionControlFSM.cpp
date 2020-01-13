//
// Created by mostafa on 29/11/2019.
//

#include <iostream>
#include "CongestionControlFSM.h"
using namespace std;
void CongestionControlFSM::update_FSM(int event) {
    if(this->state == SLOWSTART){
        this->slow_start_update(event);
    } else if(this->state == CONGESTIONAVOIDANCE){
        this->congestion_avoidance_update(event);
    } else if(this->state == FASTRECOVERY){
        this->fast_recovery_update(event);
    }
}

void CongestionControlFSM::slow_start_update(int event) {
    if(event == NEWACK){
        this->cwnd ++;
        this->dupACKCount = 0;
        if(this->cwnd*MAXSIZE >= ssthreash){
//            cout << "congestion control\n";
            this->state = CONGESTIONAVOIDANCE;
        }
    } else if(event == DUPACK){
        this->dupACKCount ++;
        if(dupACKCount == 3){
            ssthreash = cwnd*MAXSIZE/2;
            cwnd = ssthreash/MAXSIZE + 3;
            state = FASTRECOVERY;
//            cout << "fast recovery\n";
        }
    } else if(event == TIMEOUT){
        ssthreash = cwnd*MAXSIZE/2;
        cwnd = 1;
        dupACKCount = 0;
    }
}

void CongestionControlFSM::congestion_avoidance_update(int event) {
    if(event == NEWACK){
        dupACKCount = 0;
        cwnd = cwnd + (1/ cwnd);

    } else if(event == DUPACK){
        dupACKCount++;
        if(dupACKCount == 3){
            ssthreash = cwnd*MAXSIZE/2;
            cwnd = ssthreash + 3;
            state = FASTRECOVERY;
//            cout << "fast recovery\n";
        }
    } else if(event == TIMEOUT){
        ssthreash = cwnd*MAXSIZE/2;
        cwnd = 1;
        dupACKCount = 0;
        state = SLOWSTART;
//        cout << "slow start\n";
    }
}

void CongestionControlFSM::fast_recovery_update(int event) {
    if(event == NEWACK){
        cwnd = ssthreash;
        dupACKCount = 0;
        state = CONGESTIONAVOIDANCE;
//        cout << "congestion control\n";
    } else if(event == DUPACK){
        cwnd++;
    } else if(event == TIMEOUT){
        ssthreash = cwnd*MAXSIZE/2;
        cwnd = 1;
        dupACKCount = 0;
        state = SLOWSTART;
//        cout << "slow start\n";
    }
}