#include <stdio.h>
#include <iostream>
#include <string> 
#include <fstream>
#include <thread>
#include <mutex>
#include <stdlib.h>
#include <sstream>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>

using namespace std;

int clientSendSocket = -1;
void connectToClient(string addr, int port) {
	//create socket
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1) {
		//error
		std::cerr << "Could not create a socket" << std::endl;
		throw "error";
	}

	//bind socket to IP address
	// we first need to create the sockaddr obj.
	sockaddr_in address; //in means IP4
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, addr, &address.sin_addr);

	if (connect(socketfd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw "\nConnection Failed \n";

	clientSendSocket = socketfd;
}
void send_value_update(string key, float newVal) {
	stringstream ss;
	ss << "set " << key << " " << newVal;
	string s = ss.str();
	send(clientSendSocket, s.c_str(), s.length(), 0);
}

std::map<std::string, float> allAcceptingVars;
std::mutex acceptVarMapLock;
void listenOnThread(int new_socket) {
	char buffer[1024];

	string keys[] = { "/instrumentation/airspeed-indicator/indicated-speed-kt", "/instrumentation/altimeter/indicated-altitude-ft", "/instrumentation/altimeter/pressure-alt-ft", "/instrumentation/attitude-indicator/indicated-pitch-deg", "/instrumentation/attitude-indicator/indicated-roll-deg", "/instrumentation/attitude-indicator/internal-pitch-deg", "/instrumentation/attitude-indicator/internal-roll-deg", "/instrumentation/encoder/indicated-altitude-ft", "/instrumentation/encoder/pressure-alt-ft", "/instrumentation/gps/indicated-altitude-ft", "/instrumentation/gps/indicated-ground-speed-kt", "/instrumentation/gps/indicated-vertical-speed", "/instrumentation/heading-indicator/indicated-heading-deg", "/instrumentation/magnetic-compass/indicated-heading-deg", "/instrumentation/slip-skid-ball/indicated-slip-skid", "/instrumentation/turn-indicator/indicated-turn-rate", "/instrumentation/vertical-speed-indicator/indicated-speed-fpm", "/controls/flight/aileron", "/controls/flight/elevator", "/controls/flight/rudder", "/controls/flight/flaps", "/controls/engines/engine/throttle", "/engines/engine/rpm" };
	while (1) {
		int valread = read(new_socket, buffer, 1024);
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
				allAcceptingVars[keys[iW]] = stof(curBuf);
				curBuf = endBuf + 1;
				endBuf++;
				iW++;
			}
		}// next char

		cout << allAcceptingVars["/controls/engines/engine/throttle"];
	}
}

void openListeningThread(int port) {
	//create socket
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1) {
		//error
		std::cerr << "Could not create a socket" << std::endl;
		throw "error";
	}

	//bind socket to IP address
	// we first need to create the sockaddr obj.
	sockaddr_in address; //in means IP4
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
	address.sin_port = htons(port);

	//the actual bind command
	if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
		std::cerr << "Could not bind the socket to an IP" << std::endl;
		throw "error";
	}

	//making socket listen to the port
	if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
		std::cerr << "Error during listening command" << std::endl;
		throw "error";
	}
	else {
		std::cout << "Server is now listening ..." << std::endl;
	}

	// accepting a client
	int client_socket = accept(socketfd, (struct sockaddr *)&address, (socklen_t*)&address);
	if (client_socket == -1) {
		std::cerr << "Error accepting client" << std::endl;
		throw "error";
	}
	
	// Start a thread
	std::thread dataThread(listenOnThread, client_socket);
	// Done :)
}

class update_sim {
private:
	string key;
public:
	update_sim(string _key) : key(_key) {};
	void update_value(float newVal) {
		send_value_update(key, newVal);
	}
};

class accept_sim {
private:
	string key;
public:
	accept_sim(string _key) : key(_key) {};
	float get_value() {
		const std::lock_guard<std::mutex> lock(acceptVarMapLock);
		return allAcceptingVars[key];
	}
};

string rtrim_str(string s) {
	int ind = 0;
	while (ind < s.length() && s[ind] == ' ') ind++;
	s = s.substr(ind, s.length() - ind);
	return s;
}

int main(int argc, char *argv[]) { 
	if (argc != 2 && 0) {
		cout << "Usage: ./a.out commands_filename" << endl;
		return 0;
	}
	
	char *fname = argv[1];
	// Open it
	std::ifstream ifs;
	ifs.open(fname);
	if (ifs.fail())
		throw "Failed to read";

	map<string, update_sim> update_vars;
	map<string, accept_sim> accept_vars;
	map<string, float> value_vars;

	std::string line;
	while (getline(ifs, line)) {
		line = rtrim_str(line);
		// Ignore empty lines
		if (line.empty()) continue;
		
		// Parse the line
		// Commands
		if (line.rfind("openDataServer(") == 0) {
			string s = line.substr(15, line.length() - 16);
			openListeningThread(stof(s));
			continue;
		}
		if (line.rfind("connectControlClient(") == 0) {
			// get the two items
			string s = line.substr(21, line.length() - 22);
			size_t ind = line.rfind(",");
			// Address (remove quotes) + port 
			string addr = s.substr(1, ind - 1);
			int port = stoi(s.substr(ind + 1));
			// Connect to them
			connectToClient(addr, port);
			continue;
		}
		// Var?
		if (line.rfind("var ") == 0) {
			line = line.substr(4, line.length() - 4);
			// Find the space
			int ind = line.rfind(' ');
			string varName = line.substr(0, ind);
			// Trim that, what are we looking at?
			line = line.substr(ind + 1, line.length() - ind - 1);
			// Equals?
			if (line[0] == '=') {
				// remove the equals
				line = rtrim_str(line.substr(1, line.length() - 1));
				// number?
				try {
					float val = stof(line);
					// TODO: remove key from others
					value_vars[varName] = val;
				}
				catch (std::invalid_argument) {
					// TODO: check that it exists
					value_vars[varName] = accept_vars[varName].get_value();
				}

			}
			// Update sim?
			else if (line[0] == '-' && line[1] == '>') {
				if (line.rfind("-> sim(") != 0)
					throw "Error 2";
				// Remove the sim, set that (remove quotes)
				// TODO: remove key from others
				update_vars[varName] = update_sim(line.substr(8, line.length() - 8));
			}
			else if (line[0] == '<' && line[1] == '-') {
				if (line.rfind("<- sim(") != 0)
					throw "Error 2";
				// Remove the sim, set that (remove quotes)
				// TODO: remove key from others
				accept_vars[varName] = accept_sim(line.substr(8, line.length() - 8));
			}
			else throw "Error";
			continue;
		}// end of var
		// sleep/print?
		if (line.rfind("Sleep(") == 0) {
			int sleepTime = stoi(line.substr(6, line.length() - 7));
			_sleep(sleepTime);
			continue;
		}
		if (line.rfind("Print(") == 0) {
			string sToPrint = line.substr(6, line.length() - 7);
			if (sToPrint[0] == '"')
				cout << sToPrint.substr(1, sToPrint.length() - 1) << endl;
			else {
				if (accept_vars.find(sToPrint) != accept_vars.end())
					cout << accept_vars[sToPrint].get_value() << endl;
				else // TODO: check exists
					cout << value_vars[sToPrint] << endl;
			}	
		}



	}

	
}
