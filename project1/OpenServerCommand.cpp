#define WINDOWS_USE

//
// Created by noy on 
//
#include <mutex>
#include "OpenServerCommand.h"
#include "VarCommand.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#ifndef WINDOWS_USE
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#endif
using namespace std;
OpenServerCommand::OpenServerCommand() {}
unordered_map<string, double> OpenServerCommand::allAcceptingVars;
mutex OpenServerCommand::acceptVarMapLock;

void RunServer(int socketfd, unordered_map <std::string, double>  *allAcceptingVars, mutex *acceptVarMapLock);

int OpenServerCommand::execute(vector<string> commands, int ind) {
    string port  = commands[ind];
	int socketfd = -1;
#ifndef WINDOWS_USE
	//create socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    //****************
    serverSocket = socketfd;
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
    address.sin_port = htons(5402);

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
#endif
    thread t(RunServer, socketfd, &allAcceptingVars, &acceptVarMapLock);
    return 1;
}


void RunServer(int socketfd, unordered_map <std::string, double>  *allAcceptingVars, mutex *acceptVarMapLock) {

    vector<string> keys = { "/instrumentation/airspeed-indicator/indicated-speed-kt",
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
#ifdef WINDOWS_USE
	while (true) {
		for (unsigned int iKey = 0; iKey < keys.size(); iKey++) {
			acceptVarMapLock->lock();
			(*allAcceptingVars)[keys[iKey]] = 0;
			acceptVarMapLock->unlock();
		}
		return;
	}

#else 
    char buffer[1024];
	while (true) {
		// get information from the simulator
		int valread = read(socketfd, buffer, 1024);
		if (valread) break;
		// Split by comma
		// Put them in one at a time
		int iW = 0;
		char *curBuf = buffer;
		char *endBuf = curBuf;
		while (*++endBuf) {
			if (*endBuf == ',') {
				*endBuf = 0;
				// Store it

				//convert sting to float
				acceptVarMapLock->lock();
				(*allAcceptingVars)[keys[iW]] = stod(curBuf);
				acceptVarMapLock->unlock();

				/*// noy, update the var obj in "fromServer" map
				fromServer[keys[iW]]->setValue(stof(curBuf));
				// update toServerMap
				string varName = fromServer[keys[iW]]->getName();
				toServer[varName] = fromServer[keys[iW]];
				//end noy*/
				curBuf = endBuf + 1;
				endBuf++;
				iW++;
			}
		}// next char
        //cout << (*allAcceptingVars)["/controls/engines/engine/throttle"];
    }
#endif
}