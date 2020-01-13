//
// Created by mostafa on 29/11/2019.
//

#ifndef TCP_SERVER_CONGESTIONCONTROLFSM_H
#define TCP_SERVER_CONGESTIONCONTROLFSM_H
#define SLOWSTART 0
#define CONGESTIONAVOIDANCE 1
#define FASTRECOVERY 2
#define DUPACK 3
#define TIMEOUT 4
#define NEWACK 5
#define MAXSIZE 500

class CongestionControlFSM {
private:
    int dupACKCount = 0;
    float cwnd = 1;
    float ssthreash = 16000;
    int state = SLOWSTART;
    void slow_start_update(int event);
    void congestion_avoidance_update(int event);
    void fast_recovery_update(int event);
public:
    CongestionControlFSM()= default;
    int get_cwnd(){
        return int(this->cwnd);
    }
    int get_threshold(){
        return int(this->ssthreash / 500);
    }
    void update_FSM(int event);
};


#endif //TCP_SERVER_CONGESTIONCONTROLFSM_H
