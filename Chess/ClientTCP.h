#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <cstring>

#pragma comment (lib, "ws2_32.lib")

#define BUFLEN 512 // lunghezza del messaggio
#define PORT 666 // porta del server
#define SERVER "127.0.0.1" // indirizzo ip del server

using namespace std;

class ClientTCP
{
private:
	int iResult;
	WSADATA data;
	SOCKET clientSocket;
	sockaddr_in serverAddr;

public:
	ClientTCP();
	bool initializeSocket();
	bool updateServerInfo();
	bool Send(string mess);
	string Recieve();
	void SendRecieve();

};

