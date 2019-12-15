#include <stdio.h>
#include <iostream>
#include <string> 
#include <fstream>
#include <thread>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

void openThread(string port) {
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(stoi(port));

	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	while (1) {
		valread = read(new_socket, buffer, 1024);
		cout << "%s\n" << buffer;
	}
}

int main(int argc, char *argv[]) { 
	if (argc != 2) {
		cout << "Usage: ./a.out commands_filename" << endl;
		return;
	}
	openThread("5400");
	return;
	char *fname = argv[1];
	// Open it
	std::ifstream ifs;
	ifs.open(fname);
	if (ifs.fail())
		throw "Failed to read";

	std::string line;
	while (getline(ifs, line)) {
		// Ignore empty lines
		if (line.empty()) continue;

		// Parse the line
		// Commands
		if (line.rfind("openDataServer(")) {
			string s = line.substr(15, line.length() - 16);
			std::thread dataThread(openThread, s);
		}
	}

	
}
