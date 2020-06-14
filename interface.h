/* Common Interface for windows.c and linux.c */

void execute_command(int sockfd, const char *argument);

void delete_file(int sockfd, const char *argument);

void system_info(int sockfd);

void networking_information(int sockfd);

const char *parse_feature(char input[]);

const char *parse_argument(char input[]);
