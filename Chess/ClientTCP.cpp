#include "ClientTCP.h"

ClientTCP::ClientTCP()
{
	this->iResult = 0;
	this->data = {};
	this->clientSocket = 0;
	this->serverAddr = {};
}

bool ClientTCP::initializeSocket()
{
    iResult = WSAStartup(MAKEWORD(2, 2), &data);
    if (iResult != 0)
        return false;

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        WSACleanup();
        return false;
    }

	return this->updateServerInfo();
}

bool ClientTCP::updateServerInfo()
{
    // aggiorno le informazioni del server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER, &serverAddr.sin_addr);

    // Mi connetto al server tramite la socket (mando una richiesta, sperando il server la accetti)
    iResult = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR)
    {
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool ClientTCP::Send(string mess)
{
    char message[BUFLEN] = {};
    // copio il messaggio nel buffer da inviare
    strcpy_s(message, mess.c_str());
    // aggiungi manualmente un carattere di nuova riga al termine del messaggio
    strcat_s(message, "\n");
    // invio il messaggio al server
    iResult = send(clientSocket, message, (int)strlen(message), 0);
    if (iResult == SOCKET_ERROR)
    {
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }
    // attendo un po' altrimenti il server non fa in tempo a mandarmi tutta la risposta ma ricevo solo gli ultimi bytes
    Sleep(150);

    return true;
}

string ClientTCP::Recieve()
{
    char buffer[BUFLEN] = {};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    string risposta = buffer;
    // rimuovo tutti i caratteri nulli alla fine del messaggio
    risposta.erase(risposta.find_last_of(" \r\n\t") + 1);

    if (iResult > 0)
    {
        return risposta;
    }
    else if (iResult == 0)
    {
        return "\0";
    }
    else
    {
        closesocket(clientSocket);
        WSACleanup();
        return "\0";
    }
}