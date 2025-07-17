#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0601


#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <thread>
#include <string.h>
#include <WinSock2.h>
#include <process.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>


#define PORT 8080
#define INVALID_SOCK_READ 500
#define ADDRESS L"127.0.0.1"
#define MAX_CLIENTS 20



int findFirstInstanceOfInvalid(SOCKET conClients[]);
void clientListener(SOCKET conClients[], SOCKET servSocket, HANDLE console, WORD sColor, WORD fColor);


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
    InetPton(AF_INET, ADDRESS, &service.sin_addr.s_addr);
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

       
    SOCKET clientList[MAX_CLIENTS] = {INVALID_SOCKET}; // will hold all of the clients, basically the accept client


   

    /*
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
    */


    std::thread t1(clientListener, clientList, serverSocket, hConsole,SUCCESS_COLOR, ERROR_COLOR);
    t1.detach();
    char recvBuf[200];
    bool quit = false;
    while (!quit) {
        int indexOfFirstInvalid = findFirstInstanceOfInvalid(clientList);
        // Here is where we'll handle taking messages to each client and other clerical functions, so both threads handling the client list, will need to be paused.
        for (int i = 0; i < indexOfFirstInvalid - 1; i++) {
            if (clientList[i] != INVALID_SOCKET) {
                std::cout << "USER (" << i << "): DEFINED\n";
            }
        }
        


        /*
        int byteCount = recv(acceptSocket, recvBuf, 200, 0);
        if (byteCount > 0) {
            SetConsoleTextAttribute(hConsole, bgColor | FOREGROUND_BLUE);
            std::cout << "SERVER STATUS: MESSAGE RECIEVED:" << recvBuf << "\n" << "BYTECOUNT: " << byteCount << "\n";
        }
        else {
            std::cout << "BYTECOUNT IS ZERO CONN CLOSED...\n";
            WSACleanup();
        }

        if (recvBuf[0] == '/' && recvBuf[1] == 'q') {
            quit = true;
        }

        char retMsg[400] = "SERVER: RECIEVED MSG:";
        std::strcat(retMsg, recvBuf);
        byteCount = send(acceptSocket, retMsg, 400, 0);
        if (byteCount > 0) {
            std::cout << "Message sent: " << retMsg << "\n";
        }
        else {
            WSACleanup();
        }
        */
    }

    
    // Close ts at the end.
    SetConsoleTextAttribute(hConsole, SUCCESS_COLOR);
    system("pause");
    WSACleanup();
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    ExitProcess(EXIT_SUCCESS);
}



int findFirstInstanceOfInvalid(SOCKET conClients[]) {
    for (int i = 0; i < MAX_CLIENTS - 1; i++) {
        if (conClients[i] == INVALID_SOCKET) {
            return i;
        }
        else {
            continue;
        }
    }
    return MAX_CLIENTS;
} 




void clientListener(SOCKET conClients[MAX_CLIENTS], SOCKET servSocket,HANDLE console , WORD sColor, WORD fColor) {

    // TODO: Handle server being full and rejecting connections.
    // Temporary, include logic for handling disconnections after the server is "full" will be added after this test works.
    while (conClients[MAX_CLIENTS - 1] == INVALID_SOCKET) {

        std::cout << "LISTENING FOR CLIENTS\n";
        if (listen(servSocket, SOMAXCONN) == SOCKET_ERROR) {
            SetConsoleTextAttribute(console, fColor);
            std::cout << "ERROR LISTENING TO SOCKET: " << WSAGetLastError() << "\n";
        }
        else {
            SetConsoleTextAttribute(console, sColor);
            std::cout << "SOCKET LISTENING " << "\n";
        }

        // Include the client into our client list.
        int indexOfInvalid = findFirstInstanceOfInvalid(conClients);
        if (indexOfInvalid == MAX_CLIENTS) {

        }
        else {
            conClients[indexOfInvalid] = accept(servSocket, NULL, NULL);
        }
        


    }
}