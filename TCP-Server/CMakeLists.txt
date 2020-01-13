cmake_minimum_required(VERSION 3.15)
project(TCP_Server)

set(CMAKE_CXX_STANDARD 14)

add_executable(TCP_Server main.cpp packet.h ack_packet.h FSM.cpp FSM.h Server.cpp Server.h CongestionControlFSM.cpp CongestionControlFSM.h UDP_Socket.cpp UDP_Socket.h)