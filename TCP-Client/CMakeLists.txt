cmake_minimum_required(VERSION 3.15)
project(TCP_Client)

set(CMAKE_CXX_STANDARD 14)

add_executable(TCP_Client main.cpp ack_packet.h packet.h FSM.h FSM.cpp Client.cpp Client.h)