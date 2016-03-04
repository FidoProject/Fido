#include "Connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

Connection::Connection()  {
    int tempSockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    tempSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (tempSockfd < 0)
       std::cout << "ERROR opening socket";
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->PORT);
    if (bind(tempSockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
    		std::cout << "ERROR on binding";
    listen(tempSockfd,5);
    clilen = sizeof(cli_addr);
    socketFd = accept(tempSockfd,
                (struct sockaddr *) &cli_addr,
                &clilen);
    if (socketFd < 0)
    	std::cout << "ERROR on accept";
}

double Connection::getReward() {

}

std::string Connection::getString() {
	std::string newString = "";
	char buffer;

	// Keep reading char by char until a newline
	while(true) {
		read(socketFd, &buffer, 1);

		if (buffer == '\n') break;
		else newString += buffer;
	}

	return newString;
}
