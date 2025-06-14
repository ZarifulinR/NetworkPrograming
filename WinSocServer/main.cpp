#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#endif // !WIN32_LEAN_AND_MEAN


#include<Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include<stdio.h>
#include <iphlpapi.h>
#include <iostream>
#include<thread>
#include<vector>

using namespace std;

#pragma comment(lib,"Ws2_32.lib")
#define SOCK_IP_ADDR "127.0.0.1"
#define DEFAULT_PORT "27016"
#define DEFAULT_BUFFLEN 1024

void main()
{
	setlocale(LC_ALL, "");
	cout << " Hello Server \n " << endl;;
	WSAData wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		cout << "WSAStartup failed: %d\n" << iResult << endl;;
		return;
	}
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	addrinfo* result = NULL;
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		cout << "������ ������ ������� ��������" << iResult << endl;
		return;
	}



	SOCKET  ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << " Error create socket " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	iResult = bind(ListenSocket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		cout << "bind failed with error: %d\n", WSAGetLastError();
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		cout << "Error: Listen failed with code " << WSAGetLastError() << endl;
		closesocket(ListenSocket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}




	//SOCKET ClientSoket = accept(ListenSocket, NULL, NULL);

	VOID WINAPI HandleClient(SOCKET ClientSoket);
	CONST INT MAX_CLIENTS = 5;
	SOCKET clients[MAX_CLIENTS] = {};
	DWORD dwThreadIDs[MAX_CLIENTS] = {};
	HANDLE hTreads[MAX_CLIENTS] = {};

	INT i = 0;
	while (i < 5)
	{
		SOCKET ClientSoket = accept(ListenSocket, NULL, NULL);
		clients[i] = ClientSoket;
		hTreads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleClient, (LPVOID)clients[i], 0, &dwThreadIDs[i]);
		i++;

	}
	//HandleClient(ClientSoket);
	closesocket(ListenSocket);
	freeaddrinfo(result);
	WSACleanup();
}
VOID WINAPI HandleClient(SOCKET ClientSoket)
{
	INT iResult = 0;
	CHAR recvbuf[DEFAULT_BUFFLEN];
	int recv_buffer_lenth = DEFAULT_BUFFLEN;

	do
	{
		ZeroMemory(recvbuf, sizeof(recvbuf));
		iResult = recv(ClientSoket, recvbuf, recv_buffer_lenth, 0);
		if (iResult > 0)
		{
			cout << "bytes received " << iResult << endl;
			cout << " Message: " << recvbuf << endl;
			CHAR sz_responce[] = " Hello I am Server!";
			//INT iSendResult = send(ClientSoket, "Hello i am Server", strlen(recvbuf), 0);
			INT iSendResult = send(ClientSoket, recvbuf, strlen(recvbuf), 0);
			if (iSendResult == SOCKET_ERROR)
			{
				cout << "Error Send failed hith code: " << WSAGetLastError() << endl;
				closesocket(ClientSoket);
				//closesocket(ListenSocket);
				//freeaddrinfo(result);
				//WSACleanup();
				return;
			}
			cout << "Bytes sent: " << iSendResult << endl;
		}
		else if (iResult == 0)
		{
			cout << " Connection closing..." << endl;
			closesocket(ClientSoket);
		}
		else
		{
			cout << " Error recv() failed with code" << WSAGetLastError() << endl;
			closesocket(ClientSoket);
			//return;

		}

	} while (iResult > 0);

}