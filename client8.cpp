#include <iostream>
#include <conio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

void sendCoordinates(SOCKET sock, int x, int y) {
    char buffer[10];
    sprintf_s(buffer, "%03d %03d", x, y);
    cout << "Sending coordinates: " << buffer << endl;
    send(sock, buffer, strlen(buffer), 0);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed!" << endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Connection failed! Error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server!" << endl;

    int x = 10, y = 10;
    while (true) {
        system("cls");
        cout << "Smailik at: (" << x << ", " << y << ")" << endl;
        int key = _getch();

        if (key == 224) { 
            key = _getch(); 
            switch (key) {
            case 72: y = (y > 0) ? y - 1 : y; break; //pgup
            case 80: y = (y < 24) ? y + 1 : y; break; //pgdn
            case 75: x = (x > 0) ? x - 1 : x; break; //home <
            case 77: x = (x < 79) ? x + 1 : x; break; //end >
            }
        }
        else if (key == 'q') {
            closesocket(clientSocket);
            WSACleanup();
            return 0;
        }
        sendCoordinates(clientSocket, x, y);
    }
}
