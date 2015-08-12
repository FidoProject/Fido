#ifndef NETWORKING_H
#define NETWORKING_H

#include <SFML\Network.hpp>
#include <time.h>
#include <set>
#include <iostream>
#include <Windows.h>

#include "hlt.h"

static sf::TcpSocket * connectToGame()
{
	while(true)
	{
		sf::TcpSocket * s = new sf::TcpSocket();
		s->setBlocking(true);
		std::string in;
		unsigned short portNumber;
		std::cout << "What port would you like to connect to? Please enter a valid port number: ";
		while(true)
		{
			std::getline(std::cin, in);
			std::transform(in.begin(), in.end(), in.begin(), ::tolower);
			try
			{
				portNumber = std::stoi(in);
				break;
			} catch(std::exception e)
			{
				std::cout << "That isn't a valid input. Please enter a valid port number: ";
			}
		}
		if(sf::Socket::Status::Done == s->connect(sf::IpAddress::getLocalAddress(), portNumber))
		{
			std::cout << "Successfully established contact with " << s->getRemoteAddress() << ".\n";
			return s;
		}
		std::cout << "There was a problem connecting. Let's try again: \n";
	}
}

static sf::Packet& operator<<(sf::Packet& p, const std::set<hlt::Move>& moves)
{
	for(auto a = moves.begin(); a != moves.end(); a++) p << a->l.x << a->l.y << a->d;
	return p;
}

static sf::Packet& operator>>(sf::Packet& p, hlt::Map& m)
{
	p >> m.map_width >> m.map_height;
	m.contents.resize(m.map_height);
	for(auto a = m.contents.begin(); a != m.contents.end(); a++) a->resize(m.map_width);
	for(auto a = m.contents.begin(); a != m.contents.end(); a++) for(auto b = a->begin(); b != a->end(); b++) p >> b->owner >> b->age;
	return p;
}

static void getInit(sf::TcpSocket * s, unsigned char& playerTag, unsigned char& ageOfSentient, hlt::Map& m)
{
	sf::Packet r;
	s->receive(r);
	std::cout << "Received init message.\n";
	r >> playerTag >> ageOfSentient >> m;
}

static void sendInitResponse(sf::TcpSocket * s)
{
	std::string response = "Done"; sf::Packet p;
	p << response;
	s->send(p);
	std::cout << "Sent init response.\n";
}

static void getFrame(sf::TcpSocket * s, hlt::Map& m, bool &done)
{
	sf::Packet r;
	s->receive(r);
	r >> done;
	if(done == true) return;
	r >> m;
}

static void getFinish(sf::TcpSocket * s, unsigned short &wins) {
	sf::Packet r;
	s->receive(r);
	r >> wins;
}

static void sendFrame(sf::TcpSocket * s, const std::set<hlt::Move>& moves)
{
	sf::Packet p;
	p << moves;
	s->send(p);
}

#endif