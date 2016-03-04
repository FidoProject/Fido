#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>

class Connection {
public:
	const int PORT = 80;
	int socketFd;

	Connection();
	double getReward();
private:
	std::string getString();
};


#endif
