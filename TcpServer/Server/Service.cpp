#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void main() 
{
	// Initialze winsock
	WSADATA wsdata;
	WORD vet = MAKEWORD(2, 2);

	int wsOK = WSAStartup(vet, &wsdata);
	if (wsOK != 0)
	{
		std::cerr << "Can't initialze winsock! Quitting " << std::endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket£¡ Quitting" << std::endl;
		return;
	}

	// Bind the socket to an ip address and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Wait the connection
	sockaddr_in client;
	int clientsize = sizeof(client);

	SOCKET clientsocket = accept(listening, (sockaddr*)&client, &clientsize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXHOST];		// Service (i.e. port) the client is connect on 

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXHOST, 0) == 0)
	{
		std::cout << host << "connect on port" << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << "connected on port" << 
			ntohs(client.sin_port) << std::endl;
	}

	// Close listening socket 
	closesocket(listening);

	// While loop: accept and echo messge back to clien
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);

		// Wait for client send data
		int bytesReceived = recv(clientsocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "Error in recv(). Qutting" << std::endl;
			break;
		}
		if (bytesReceived == 0)
		{
			std::cout << "Client disconnected"<< std::endl;
			break;
		}

		// Echo message back to client
		send(clientsocket, buf, bytesReceived + 1, 0);

	}


	// Close the socket
	closesocket(clientsocket);

	// Shutdown winsock
	WSACleanup;

}