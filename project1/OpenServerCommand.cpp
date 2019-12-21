//
// Created by noy on ١٩‏/١٢‏/٢٠١٩.
//
#include <mutex>
#include "OpenServerCommand.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>

using namespace std;
OpenServerCommand::OpenServerCommand(unsigned int port) : m_port(port) {}

int OpenServerCommand ::execute() {
    //create socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket"<<std::endl;
        return -1;
    }

    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    address.sin_port = htons(m_port);

    //the actual bind command
    if (bind(socketfd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) == -1) {
        std::cerr<<"Could not bind the socket to an IP"<<std::endl;
        return -2;
    }

    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr<<"Error during listening command"<<std::endl;
        return -3;
    } else{
        std::cout<<"Server is now listening ..."<<std::endl;
    }

    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *)&address,
                               (socklen_t*)&address);

    if (client_socket == -1) {
        std::cerr<<"Error accepting client"<<std::endl;
        return -4;
    }
    //close(socketfd); //closing the listening socket

    //reading from client
    char buffer[1024] = {0};
    int valread = read(socketfd , buffer, 1024);
    std::cout<<buffer<<std::endl;

    //writing back to client
    char hello[] = "Hello, I can hear you! \n";
    send(client_socket , hello , strlen(hello) , 0 );
    std::cout<<"Hello message sent\n"<<std::endl;
    string keys[] = { "/instrumentation/airspeed-indicator/indicated-speed-kt",
                      "/instrumentation/altimeter/indicated-altitude-ft",
                      "/instrumentation/altimeter/pressure-alt-ft",
                      "/instrumentation/attitude-indicator/indicated-pitch-deg",
                      "/instrumentation/attitude-indicator/indicated-roll-deg",
                      "/instrumentation/attitude-indicator/internal-pitch-deg",
                      "/instrumentation/attitude-indicator/internal-roll-deg",
                      "/instrumentation/encoder/indicated-altitude-ft",
                      "/instrumentation/encoder/pressure-alt-ft",
                      "/instrumentation/gps/indicated-altitude-ft",
                      "/instrumentation/gps/indicated-ground-speed-kt",
                      "/instrumentation/gps/indicated-vertical-speed",
                      "/instrumentation/heading-indicator/indicated-heading-deg",
                      "/instrumentation/magnetic-compass/indicated-heading-deg",
                      "/instrumentation/slip-skid-ball/indicated-slip-skid",
                      "/instrumentation/turn-indicator/indicated-turn-rate",
                      "/instrumentation/vertical-speed-indicator/indicated-speed-fpm",
                      "/controls/flight/aileron", "/controls/flight/elevator",
                      "/controls/flight/rudder",
                      "/controls/flight/flaps",
                      "/controls/engines/engine/throttle",
                      "/engines/engine/rpm" };
    mutex acceptVarMapLock;
    while (true) {
        // get information from the simulator
         valread = read(socketfd, buffer, 1024);
        // Split by comma
        // Put them in one at a time
        int iW = 0;
        char *curBuf = buffer;
        char *endBuf = curBuf;
        while (*++endBuf) {
            if (*endBuf == ',') {
                *endBuf = 0;
                // Store it

                const std::lock_guard<std::mutex> lock(acceptVarMapLock);
                //convert sting to float
                allAcceptingVars[keys[iW]] = stof(curBuf);
                curBuf = endBuf + 1;
                endBuf++;
                iW++;
            }
        }// next char

        cout << allAcceptingVars["/controls/engines/engine/throttle"];
    }

}
