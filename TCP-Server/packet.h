//
// Created by mostafa on 24/11/2019.
//

#ifndef TCP_SERVER_PACKET_H
#define TCP_SERVER_PACKET_H
#define MAXSIZE 500
#include <cstdint>
#pragma pack(1)   // this helps to pack the struct to 5-bytes
struct packet{
    uint16_t len;
    uint32_t seqno;
    bool moreFragments;
    char data[MAXSIZE];
};
#pragma pack(0)   // turn packing off
#endif //TCP_SERVER_PACKET_H
