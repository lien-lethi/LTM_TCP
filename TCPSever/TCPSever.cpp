#include <string>
#include "iostream"
#include <ws2tcpip.h>
using namespace std;
#pragma comment (lib,"ws2_32.lib")
void main()
{
	string ipAddress = "127.0.0.1";
	int port = 54000;
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		cerr << "Can't initialize winsock! Quitting" << endl;
		return;
	}
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

	sockaddr_in client;
	int clientsize = sizeof(client);
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port" << ntohs(client.sin_port) << endl;

	}
	cout << "<Dia chi IP: " << ipAddress << ">" << endl;
	
	closesocket(listening);
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cout << "client disconnected" << endl;
			break;
		}
		cout << "<Cong: " << port << "> " <<" <Hello Client, What can we do for you?> ";
		cout <<" <" << string(buf, 0, bytesReceived) << ">" << endl;
		send(clientSocket, buf, bytesReceived + 1, 0);
			
	}
	closesocket(clientSocket);
	WSACleanup();
}