#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 创建套接字文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置套接字选项，避免端口占用错误
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定套接字到指定端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听套接字
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    while (1)
    {
        // 接受客户端连接
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            continue; // 继续监听其他连接，而不是退出
        }

        printf("Connected to client\n");

        while (1)
        {
            char buffer[1024] = {0};
            long valread = read(new_socket, buffer, 1024);
            if (valread <= 0)
            {
                // 客户端断开连接或发生错误
                printf("Client disconnected or error occurred\n");
                break; // 退出内部循环，等待新的连接
            }

            printf("[INFO]: Server received: %s\n", buffer);
            char *serverRespond = "OK";
            send(new_socket, serverRespond, strlen(serverRespond), 0);
            printf("[INFO] Server responded: %s\n", serverRespond);
        }

        close(new_socket); // 关闭这个连接的套接字
    }

    return 0;
}
