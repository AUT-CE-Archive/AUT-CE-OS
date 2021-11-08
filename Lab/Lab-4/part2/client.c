#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 2014

void *listener(void *_sock);

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error.\n");
        return -1;
    } else
        printf("Socket created successfully.\n");

    // set memory cells to zero
    memset(&serv_addr, '0', sizeof(serv_addr));

    // configure port and address family
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address / Address not supported.\n");
        return -1;
    } else
        printf("Address 127.0.0.1 available and supported.\n");

    // establish connection to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection to server Failed.\n");
        return -1;
    } else
        printf("Connected to server successfully.\n");

    // create a thread to listen to server
    pthread_t tid;
    if (pthread_create(&tid, NULL, listener, (void *)sock) != 0) {
        printf("\nUnable to create thread.\n");
        exit(-1);
    } else
        printf("Awaiting your request..\n");

    // forever sending
    while (1)
    {
        char transmission[BUFFER_SIZE];
        fgets(transmission, BUFFER_SIZE, stdin);

        // quit
        if (strcmp(transmission, "quit\n") == 0)
            break;
        else
            send(sock, transmission, strlen(transmission), 0);
    }

    pthread_cancel(tid);        // remove thread
    shutdown(sock, SHUT_RDWR);  // shutdown socket
    close(sock);                // close socket
    printf("[CLIENT] Connection terminated, Socket closed.\n");
    return 0;
}

// [Thread handler] listens to transmissions
void *listener(void *_sock)
{   
    int sock = (int)(long)_sock, valread;
    char buffer[BUFFER_SIZE] = {0};

    // listen to server forever while there is a transmission
    while ((valread = read(sock, buffer, BUFFER_SIZE)) > 0) {
        printf("%s\n", buffer);
        bzero(buffer, strlen(buffer));
    }
}