#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080

void error_handler(int status, char error_message[50]);

int main(int argc, char const *argv[])
{
    char buffer[1024] = {0};

    // creates socket file descriptor
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    error_handler(server_fd, "Socket creation failed.");
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // host to network -- coverts the ending of the given integer
    const int addrlen = sizeof(address);

    // binding
    error_handler(bind(server_fd, (struct sockaddr *)&address, sizeof(address)), "Binding failed.");
    printf("Binded successfully.\n");

    // listening on server socket with backlog size 3.
    error_handler(listen(server_fd, 3), "Cannot listen to incoming transmissions.");    
    printf("Listening on %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    // forever accepting incomming connections, creating threads for them and listening to their transmissions
    // while (1) {
        
    // }

    // accepting client
    // accept returns client socket and fills given address and addrlen with client address information.
    int client_socket, valread;
    if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Client %s:%d entered chatroom.\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    // reads a buffer with maximum size 1024 from socket.
    valread = read(client_socket, buffer, 1024);
    if (valread < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    
    // works like a mirror and writes to client socket
    send(client_socket, buffer, strlen(buffer), 0);
    printf("(s = %d) %s\n", valread, buffer);

    // Avoid binding failure
    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);

    return 0;
}

// an abstraction for error handling
void error_handler(int status, char error_message[50]) {
    if (status < 0) {
        perror(error_message);
        exit(EXIT_FAILURE);
    }
}