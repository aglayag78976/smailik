#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 12345

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed!" << endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed!" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    listen(serverSocket, 1);
    cout << "Waiting for connection...\n";

    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Accept failed!" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    cout << "Client connected!\n";

    int x = 10, y = 10;
    char buffer[10];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            cerr << "Connection lost or no data received!" << endl;
            break;
        }

        cout << "Received data: " << buffer << endl; 

        if (sscanf_s(buffer, "%d %d", &x, &y) != 2) {
            cerr << "Error parsing coordinates!" << endl;
            continue;
        }

        system("cls");
        cout << "Smailik at: (" << x << ", " << y << ")" << endl;
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
