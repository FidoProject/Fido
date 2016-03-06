#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>

class Connection {
public:
	const int PORT = 2004;
	int socketFd;

	Connection();
	~Connection();
	double getReward();
	void disconnect();
	std::string getString();
};


#endif
