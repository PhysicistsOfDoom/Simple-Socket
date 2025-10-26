#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 512

int main(int argc, char *argv[]) {

    // Check command line arguments
    if (argc != 4) {
        printf("Usage: %s <server IPv6 addr> <server port> <message>\n", argv[0]);
        return 1;
    }

    char *serverAddrStr = argv[1];
    char *serverPortStr = argv[2];
    char *message = argv[3];
    int serverPort = atoi(serverPortStr);

    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    // Create a TCP socket (IPv6)
    SOCKET clientSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        printf("socket() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up the server address
    struct sockaddr_in6 serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(serverPort);

    result = inet_pton(AF_INET6, serverAddrStr, &serverAddr.sin6_addr);
    if (result != 1) {
        printf("inet_pton() failed. Invalid address format.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    printf("Connecting to %s on port %d...\n", serverAddrStr, serverPort);
    result = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        printf("connect() failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    printf("Connected!\n");

    // Send the echo message
    int msgLen = (int)strlen(message);
    if (msgLen == 0) {
        printf("Message length is zero. Nothing to send.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    int bytesSent = send(clientSocket, message, msgLen, 0);
    if (bytesSent == SOCKET_ERROR) {
        printf("send() failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    printf("Sent %d bytes: %s\n", bytesSent, message);

    // Receive the echo message
    char buffer[BUFFER_SIZE];
    int totalBytesReceived = 0;
    printf("Receiving echoed message...\n");

    while (1) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            printf("%s", buffer);
            totalBytesReceived += bytesReceived;
        } else if (bytesReceived == 0) {
            // connection closed
            break;
        } else {
            printf("recv() failed: %d\n", WSAGetLastError());
            break;
        }
    }

    printf("\nTotal bytes received: %d\n", totalBytesReceived);

    // Close connection
    printf("Closing connection...\n");
    closesocket(clientSocket);

    // Clean up Winsock
    WSACleanup();

    printf("Done!\n");
    return 0;
}
