//
// Created by noy on ١٧‏/١٢‏/٢٠١٩.
//
#include <string>
#ifndef EX3_COMMAND_H
#define EX3_COMMAND_H
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#define PORT 8081
class Command {
public:
    virtual ~Command(){}
    virtual int ‪execute() = 0;‬‬
};

class OpenDataServer : public Command {
private:
    std::string port;
public:
    OpenDataServer(std::string portget){
        port = portget;
    }
    int execute() {
        /*each socket: 1.socket - crate socket. 2. bind - connect to spsific port
         * 3.listen - open the port to listen 4. accept - start */
        //create socket - socket - create an endpoint for communication
        //int socket(int domain, int type, int protocol);
        //The domain argument specifies a communication  domain;  this  selects  the
        // protocol  family which will be used for communication.\
    //These families are
        // defined in <sys/socket.h> AF_INET             IPv4 Internet protocols
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
        address.sin_port = htons(PORT);
        //we need to convert our number
        // to a number that the network understands.

        //the actual bind command
        if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
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

        close(socketfd); //closing the listening socket

        //reading from client
        char buffer[1024] = {0};
        int valread = read( client_socket , buffer, 1024);
        std::cout<<buffer<<std::endl;

        //writing back to client
        char *hello = "Hello, I can hear you! \n";
        send(client_socket , hello , strlen(hello) , 0 );
        std::cout<<"Hello message sent\n"<<std::endl;
};

class ConnectControlClient : public Command {
public:
    int execute() {
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        char *hello = "Hello from client";
        char buffer[1024] = {0};
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
            return -1;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
        {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("\nConnection Failed \n");
            return -1;
        }
        send(sock , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
        valread = read( sock , buffer, 1024);
        printf("%s\n",buffer );
    }
};;
class DfineVarCommand : public Command {
public:
    int execute(){

    }
};

#endif //EX3_COMMAND_H
