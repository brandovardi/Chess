#include "ClientTCP.h"

ClientTCP::ClientTCP()
{
	this->iResult = 0;
	this->data = {};
	this->clientSocket = 0;
	this->serverAddr = {};
}

bool ClientTCP::CreateSocketConnectServer()
{
    iResult = WSAStartup(MAKEWORD(2, 2), &data);
    if (iResult != 0)
        return false;

    this->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        WSACleanup();
        return false;
    }

    // aggiorno le informazioni del server
    this->serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER, &serverAddr.sin_addr);

    // Mi connetto al server tramite la socket (mando una richiesta, sperando il server la accetti)
    iResult = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR)
        return false;

    return true;
}

bool ClientTCP::Send(string mess)
{
    char message[BUFLEN] = {};
    // copio il messaggio nel buffer da inviare
    strcpy(message, mess.c_str());
    // aggiungi manualmente un carattere di nuova riga alla fine del messaggio
    strcat(message, "\n");
    // invio il messaggio al server
    iResult = send(clientSocket, message, (int)strlen(message), 0);
    if (iResult == SOCKET_ERROR)
        return false;

    return true;
}

string ClientTCP::RecieveCorrectData()
{
    string dataRecieved = Recieve();
    // controllo se ho ricevuto solamente un byte, allora vado a leggere tutto il rest
    if (dataRecieved.length() == 1)
        dataRecieved.append(Recieve());

    return dataRecieved;
}

string ClientTCP::Recieve()
{
    // attendo un minimo per dare il tempo di ricevere tutti i dati correttamente
    Sleep(5);
    char buffer[BUFLEN] = {};
    iResult = recv(clientSocket, buffer, sizeof(buffer), 0);
    string risposta = buffer;

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

void ClientTCP::Close()
{
    // spengo la connessione (sia per inviare che per ricevere)
    shutdown(clientSocket, SD_BOTH);
    // chiudo la socket
    closesocket(clientSocket);
    // pulisco il buffer
    WSACleanup();
    /*
    * An application or DLL is required to perform a successful WSAStartup call before
    * it can use Windows Sockets services. When it has completed the use of Windows Sockets,
    * the application or DLL must call WSACleanup to deregister itself from a Windows Sockets
    * implementation and allow the implementation to free any resources allocated on behalf
    * of the application or DLL.
    */
}
