#include <iostream>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

void main()
{
	std::string ipAddress = "127.0.0.1"; //IP Address of th servier
	int port = 54000;

	// Initialze winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start winsock,Err #" << wsResult << std::endl;
		return;
	}
		 
	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server ,Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	std::string userInput;

	do
	{
		// Prompt the user for some text
		std::cout << ">";
		std::getline(std::cin, userInput);

		if (userInput.size() > 0)
		{
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{

				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					// Echo response to console
					std::cout << "SERVER" << std::string(buf, 0, bytesReceived) << std::endl;
				}
			}
		}	
	} while (userInput.size()>0);
	
	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}
