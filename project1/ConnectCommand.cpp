#define WINDOWS_USE

//
// Created by noy on
//
#include "OpenServerCommand.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#ifndef WINDOWS_USE
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "ConnectCommand.h"
ConnectCommand::ConnectCommand() {}

int ConnectCommand::execute(vector<string> commands, int ind) {
    string local_host  = commands[ind];
    string port = commands[ind+1];
#ifndef WINDOWS_USE
    //create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    //*******************************************
    if (client_socket == -1) {
        //error
        std::cerr << "Could not create a socket"<<std::endl;
        return -1;
    }

    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr(local_host.c_str());  //the localhost address
    address.sin_port = htons(atoi(port.c_str()));
    //we need to convert our number (both port & localhost)
    // to a number that the network understands.

    // Requesting a connection with the server on local host with port 5400
    int is_connect = connect(client_socket, (struct sockaddr *)&address, sizeof(address));
    if (is_connect == -1) {
        std::cerr << "Could not connect to host server"<<std::endl;
        return -2;
    } else {
        std::cout<<"Client is now connected to server" <<std::endl;
    }

    //if here we made a connection
    char hello[] = "set controls/flight/rudder 1\r\n";
    int is_sent = send(client_socket , hello , strlen(hello) , 0 );
    if (is_sent == -1) {
        std::cout<<"Error sending message"<<std::endl;
    } else {
        std::cout<<"Hello message sent to server" <<std::endl;
    }
    /*char buffer[1024] = {0};
    int valread = read( client_socket , buffer, 1024);
    std::cout<<buffer<<std::endl;*/

    close(client_socket);
#endif
    return 2;
}