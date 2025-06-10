#pragma warning(disable:4326)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#endif // !WIN32_LEAN_AND_MEAN


#include<Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include<stdio.h>
#include <iphlpapi.h>
#include <iostream>
#pragma comment (lib,"Ws2_32.lib")
#define DEFAULT_PORT "27016"
#define DEFAULT_BUFLEN 1024
using namespace std;

void main()
{
	setlocale(LC_ALL, "");
	cout << "Hello Client" << endl;;
	cout << "Helloo  " << endl;
	WSAData wsaData;
	INT iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo* result = NULL;
	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		cout << "GetAddrinfo faled: " << iResult << endl;
		WSACleanup();
		return;
	}
	SOCKET ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "Socket creation failed with code: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	iResult = connect(ConnectSocket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		cout << "Unable to connect to server: " << endl;
		closesocket(ConnectSocket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	CHAR sendbuf[DEFAULT_BUFLEN] = "  Hello i am Client ";
	
	CHAR recvbuffer[DEFAULT_BUFLEN] = {};
	do
	{
		ZeroMemory(recvbuffer, sizeof(recvbuffer));
		iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
		if (iResult == SOCKET_ERROR)
		{
			cout << "Send faled: " << WSAGetLastError() << endl;
			closesocket(ConnectSocket);
			freeaddrinfo(result);
			WSACleanup();
			return;
		}
		cout << " Send complited " << endl;

		iResult = recv(ConnectSocket, recvbuffer, DEFAULT_BUFLEN, 0);
		if (iResult > 0)cout << " Bytes received: " << iResult << "  Message: " << recvbuffer << endl;
		else if (iResult == 0)cout << "Connection closed " << endl;
		else cout << "receive failed with code: " << WSAGetLastError() << endl;
				cout << "¬ведите сообщение:  "; cin.getline(sendbuf, DEFAULT_BUFLEN) ;

	} while (iResult > 0 && strcmp(sendbuf,"Exit"));
	iResult = shutdown(ConnectSocket, SD_SEND);
	closesocket(ConnectSocket);
	freeaddrinfo(result);
	WSACleanup();
	system("PAUSE");
}