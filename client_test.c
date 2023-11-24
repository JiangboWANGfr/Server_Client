#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // 链接Winsock库

#define PORT 8080

int main()
{
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;

    // 初始化Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Socket creation failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将IPv4地址从文本转换为二进制形式
    if (inet_pton(AF_INET, "192.168.249.7", &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    int count = 0;

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection Failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    while (1)
    {
        // sent count
        char messageSend[1024] = {0};
        snprintf(messageSend, sizeof(messageSend), "%d", count++);
        send(sock, messageSend, strlen(messageSend), 0);
        printf("[INFO] Client messageSend : %s\n", messageSend);
        char buffer[1024] = {0};
        int valread = recv(sock, buffer, 1024, 0);
        printf("[INFO] Client Receive : %s\n", buffer);
    }

    // 关闭套接字和清理Winsock
    closesocket(sock);
    WSACleanup();

    return 0;
}
