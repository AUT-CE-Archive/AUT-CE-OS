#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 2014

void error_handler(int status, char error_message[50], char success_message[100]);
void client_handler(void *_socket);

int main(int argc, char const *argv[])
{
    char buffer[BUFFER_SIZE] = {0};
    char tmp_msg[100];

    // creates socket file descriptor
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    error_handler(server_fd, "[SERVER - ERR] Socket creation failed.", "[SERVER - SUCCESS] Socket created Successfully.\n");
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // host to network -- coverts the ending of the given integer
    const int addrlen = sizeof(address);

    // binding
    error_handler(bind(server_fd, (struct sockaddr *)&address, sizeof(address)), "[SERVER - ERR] Binding failed.", "[SERVER - SUCCESS] Binded successfully.\n");

    // listening on server socket with backlog size 3.
    sprintf(tmp_msg, "[SERVER - SUCCESS] Listening on %s:%d ...\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    error_handler(listen(server_fd, 3), "[SERVER - ERR] Failed to listen incoming transmissions.", tmp_msg);

    // accepting client (accept returns client socket and fills given address and addrlen with client address information.)
    int client_socket = client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    sprintf(tmp_msg, "[SERVER - SUCCESS] Client %s:%d connected.\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    error_handler(client_socket, "[SERVER - ERR] Could not accept client.\n", tmp_msg);    

    // Welcome message from the server
    char *welcome_message = "[SERVER] Welcome Client!";
    send(client_socket, welcome_message, strlen(welcome_message), 0);

    // reads a buffer with maximum size 1024 from socket.
    int valread;
    if ((valread = read(client_socket, buffer, BUFFER_SIZE)) < 0) {
        perror("read");
        return -1;
    }
    printf("[SERVER] %s\n", buffer);

    // Avoid binding failure
    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);
    printf("[SERVER - SUCCESS] Connection terminated.\n");

    return 0;
}

// an abstraction for error handling
void error_handler(int status, char error_message[50], char success_message[50]) {
    if (status < 0) {
        perror(error_message);
        exit(EXIT_FAILURE);
    } else
        printf("%s", success_message);
}

// a client hander
void client_handler(void *_socket) {

    int sock = (int)(long)_socket;
    int valread;
    char buffer[1000];
    bzero(buffer, strlen(buffer));

    // listen to server forever
    while (1) {
        if ((valread = read(sock, buffer, BUFFER_SIZE)) < 0) {
            perror("read");
            break;
        }
        printf("[SERVER] %s\n", buffer);
    }
}