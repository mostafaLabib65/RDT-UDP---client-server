//
// Created by mostafa on 24/11/2019.
//

#ifndef TCP_SERVER_ACK_PACKET_H
#define TCP_SERVER_ACK_PACKET_H

#include <cstdint>

#pragma pack(1)   // this helps to pack the struct to 5-bytes
struct ack_packet{
    uint16_t len;
    uint32_t ackno;
};
#pragma pack(0)   // turn packing off
#endif //TCP_SERVER_ACK_PACKET_H
