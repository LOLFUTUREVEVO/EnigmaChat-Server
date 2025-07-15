#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0601

#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <tchar.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>


#define PORT 8080







int main(const int argc, const char* argv[]) {
    std::cout << "Start Program\n";
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD attributes = consoleInfo.wAttributes;
    WORD bgColor = (attributes & 0x00F0);
    WORD ERROR_COLOR = FOREGROUND_RED | bgColor;
    WORD SUCCESS_COLOR = FOREGROUND_GREEN | bgColor;



    // WSA setup
    SOCKET serverSocket, acceptSocket;
    int port = PORT;
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0) {
        SetConsoleTextAttribute(hConsole, ERROR_COLOR);
        std::cout << "WINSOCK NOT FOUND!\n";
        return 0;
    }
    else {
        SetConsoleTextAttribute(hConsole, SUCCESS_COLOR);
        std::cout << "WINSOCK FOUND!\n";
        std::cout << "STATUS: " << wsaData.szSystemStatus << "\n";
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Setting up server socket
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        SetConsoleTextAttribute(hConsole, ERROR_COLOR);
        std::cout << "Error at socket()! : " << WSAGetLastError() << "\n";
        WSACleanup();
        return 0;
    }
    else {
        SetConsoleTextAttribute(hConsole, SUCCESS_COLOR);
        std::cout << "Socket() is good!\n";
    }


    // Binding the socket
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(port);
    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        SetConsoleTextAttribute(hConsole, ERROR_COLOR);
        std::cout << "BIND FAILED: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        return 0;
    }
    else {
        SetConsoleTextAttribute(hConsole, SUCCESS_COLOR);
        std::cout << "BIND IS GOOD!\n";
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        SetConsoleTextAttribute(hConsole, ERROR_COLOR);
        std::cout << "ERROR LISTENING TO SOCKET: " << WSAGetLastError() << "\n";
    }
    else {
        SetConsoleTextAttribute(hConsole, SUCCESS_COLOR);
        std::cout << "SOCKET LISTENING " << "\n";
    }

    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        SetConsoleTextAttribute(hConsole, ERROR_COLOR);
        std::cout << "ACCEPT FAILED: " << WSAGetLastError() << "\n";
    }

    SetConsoleTextAttribute(hConsole, SUCCESS_COLOR);
    system("pause");
    WSACleanup();




    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    ExitProcess(EXIT_SUCCESS);
}