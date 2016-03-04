#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>

class Connection {
public:
	const int PORT = 2007;
	int socketFd;

	Connection();
	~Connection();
	double getReward();
	void disconnect();
private:
	std::string getString();
};


#endif
