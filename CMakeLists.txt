cmake_minimum_required(VERSION 3.30)
project(Client_Winsock_TCP C)

set(CMAKE_C_STANDARD 17)

add_executable(Client_Winsock_TCP main.c)

target_link_libraries(Client_Winsock_TCP ws2_32)