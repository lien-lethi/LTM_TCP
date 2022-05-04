#include <string>
#include <iostream>
#include <ws2tcpip.h>
using namespace std;
#pragma comment (lib,"ws2_32.lib")
void main()
{
    string ipAddress = "127.0.0.1";
    int port = 54000;
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        cerr << "Cant's start winsock, Err" << wsResult << endl;
        return;
    }
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create socket, Err" << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "can't connect to server, Err" << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }
    char buf[4096];
    string userInput;
    do {
        cout << "Nhap noi dung tin nhan gui den SERVER: ";
        getline(cin, userInput);
        if (userInput.size() > 0)
        {
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR)
            {
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0)
                {
                    cout << "<127.0.0.1> <5400> " << string(buf, 0, bytesReceived) << endl;
                }
            }
        }
    } while (userInput.size() > 0);
    closesocket(sock);
    WSACleanup();
}
