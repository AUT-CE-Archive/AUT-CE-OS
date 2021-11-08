#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 2014
#define MAX_CLIENTS 10

// Handlers
void error_handler(int status, char error_message[50], char success_message[100]);
void client_handler(void *_socket);
void client_transmission_handler(void *_socket, char buffer[BUFFER_SIZE]);
void client_transmission_send_handler(void *_socket, char *message);

// Utility functions
int str2num(char *str);
char *copyString(char s[]);

// Client struct
struct Client {
    char port[50];  // Unique port
    int socket;     // Socket
    pthread_t tid;  // Thread id
    int group_id;   // Group id
};

struct Client clients[MAX_CLIENTS];

int main(int argc, char const *argv[])
{
    char buffer[BUFFER_SIZE] = {0};
    char tmp_msg[100];

    // creates socket file descriptor
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    error_handler(server_fd, "[SERVER - ERR] Socket creation failed.", "[SERVER - SUCCESS] Socket created Successfully.\n");

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // host to network -- coverts the ending of the given integer
    const int addrlen = sizeof(address);

    // binding
    error_handler(bind(server_fd, (struct sockaddr *)&address, sizeof(address)), "[SERVER - ERR] Binding failed", "[SERVER - SUCCESS] Binded successfully.\n");

    // listening on server socket with backlog size 3.
    sprintf(tmp_msg, "[SERVER - SUCCESS] Listening on %s:%d ...\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    error_handler(listen(server_fd, 3), "[SERVER - ERR] Failed to listen incoming transmissions.", tmp_msg);

    // forever handle clients
    while (1) {

        // accepting client (accept returns client socket and fills given address and addrlen with client address information.)
        int client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        sprintf(tmp_msg, "[SERVER - SUCCESS] Client %s:%d connected.\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        error_handler(client_socket, "[SERVER - ERR] Could not accept client.\n", tmp_msg);

        // Welcome message from the server
        char *welcome_message = "[SERVER] Welcome Client!";
        send(client_socket, welcome_message, strlen(welcome_message), 0);

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_handler, (void *)client_socket) != 0)
        {
            printf("\n[SERVER - ERR] Unable to create thread for client.\n");
            exit(-1);
        }
    }

    // // Avoid binding failure
    // shutdown(client_socket, SHUT_RDWR);
    // close(client_socket);
    // printf("[SERVER] Connection terminated.\n");

    return 0;
}

// Abstraction for error handling
void error_handler(int status, char error_message[50], char success_message[50])
{
    if (status < 0)
    {
        perror(error_message);
        exit(EXIT_FAILURE);
    }
    else
        printf("%s", success_message);
}

// Client hander
void client_handler(void *_socket)
{
    int sock = (int)(long)_socket, valread;
    char buffer[BUFFER_SIZE] = {0};

    // listen to client forever while there is a transmission
    while ((valread = read(sock, buffer, BUFFER_SIZE)) > 0)
    {
        printf("[CLIENT] %s", buffer); 

        // Remove tail white spaces
        buffer[strcspn(buffer, "\n")] = 0;

        client_transmission_handler((void *)sock, buffer);
        bzero(buffer, strlen(buffer));
    }
}

// Client transmission handler
void client_transmission_handler(void *_socket, char buffer[BUFFER_SIZE]) {

    int i = 0, group_id = -1;
    char *command = '\0', *message = '\0';
    char *token = strtok(buffer, " ");

    // Split transmission into parts
    while (token != NULL) {
        if (i == 0)
            command = copyString(token);
        else if (i == 1)
            group_id = str2num(token);
        else if (i == 2 && strcmp(command, "send") == 0)
            message = copyString(token);
        
        // Step
        i++; token = strtok(NULL, " ");
    }

    // Handle commands
    if (strcmp(command, "send") == 0) {
        client_transmission_send_handler((void *)_socket, message);
    }
}

// Client transmission send handler
void client_transmission_send_handler(void *_socket, char *message) {
    int sock = (int)(long)_socket;
    
    send(sock, message, strlen(message), 0);
}


// Copy string (Avoid point)
char *copyString(char s[])
{
    char *s2;
    s2 = (char *)malloc(20);

    strcpy(s2, s);
    return (char *)s2;
}

// Convert string into integer
int str2num(char *str)
{
    int result = 0, puiss = 1;

    while (('-' == (*str)) || ((*str) == '+')) {
        if (*str == '-')
            puiss = puiss * -1;
        str++;
    }
    while ((*str >= '0') && (*str <= '9')) {
        result = (result * 10) + ((*str) - '0');
        str++;
    }
    return (result * puiss);
}