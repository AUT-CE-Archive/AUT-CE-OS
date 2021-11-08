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
void client_handler(void *socket);
void client_transmission_handler(int sock, char buffer[BUFFER_SIZE]);
void client_transmission_send_handler(int sock, int group_id, char *message);
void client_transmission_group_handler(int sock, int group_id);

// Utility functions
int str2num(char *str);
char *copyString(char s[]);
struct Client get_client(int sock);

// Client struct
struct Client {
    char name;             // Name
    int socket;            // Socket
    int group_id;          // Group id
};

struct Client clients[MAX_CLIENTS];
int clients_array_pointer = 0;      // Keeps track of clients array head

int main(int argc, char const *argv[])
{
    char buffer[BUFFER_SIZE] = {0};     // Buffer    
    char tmp_msg[100];                  // Temp string to store error messages

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

        // Create thread
        pthread_t tid;
        if (pthread_create(&tid, NULL, client_handler, (void *)client_socket) != 0)
        {
            printf("\n[SERVER - ERR] Unable to create thread for client.\n");
            exit(-1);
        }

        // Add client struct to the array
        clients[clients_array_pointer].name = (clients_array_pointer + 65);  // A letter as a name
        clients[clients_array_pointer].socket = client_socket;
        clients[clients_array_pointer].group_id = -1;
        clients_array_pointer++;

        // Welcome message from the server
        sprintf(tmp_msg, "[SERVER] Welcome client '%c'!", clients[clients_array_pointer - 1].name);
        send(client_socket, tmp_msg, strlen(tmp_msg), 0);
    }

    // // Avoid binding failure
    // shutdown(client_socket, SHUT_RDWR);
    // close(client_socket);
    // printf("[SERVER] Connection terminated.\n");

    return 0;
}

// Client hander
void client_handler(void *socket) {
    int valread, sock = (int)(long)socket;
    char buffer[BUFFER_SIZE] = {0};    

    // listen to client forever while there is a transmission
    while ((valread = read(sock, buffer, BUFFER_SIZE)) > 0)
    {
        // Get sender name
        char sender_name = get_client(sock).name;     

        printf("[CLIENT %c] %s", sender_name, buffer); 

        // Remove tail white spaces
        buffer[strcspn(buffer, "\n")] = 0;

        client_transmission_handler((void *)sock, buffer);
        bzero(buffer, strlen(buffer));        
    }
}

// Client transmission handler
void client_transmission_handler(int sock, char buffer[BUFFER_SIZE]) {

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
    if (strcmp(command, "send") == 0)
        client_transmission_send_handler(sock, group_id, message);
    else if (strcmp(command, "join") == 0)
        client_transmission_group_handler(sock, group_id);
    else if (strcmp(command, "leave") == 0)
        client_transmission_group_handler(sock, -1);
}

// Handles the message broadcasting in groups
void client_transmission_send_handler(int sock, int group_id, char *message) {

    int i;
    char transmission[BUFFER_SIZE];
    char sender_name = get_client(sock).name;   // Get sender name
    
    // Send transmission to all clients in the same group as sender    
    sprintf(transmission, "[CLIENT %c] %s", sender_name, message);
    for (i = 0; i < clients_array_pointer; i++)
        if (clients[i].group_id == group_id && clients[i].name != sender_name)  // Send to all clients in the group except the sender itself
            send(clients[i].socket, transmission, strlen(transmission), 0);
}

// Handles the group joining command
void client_transmission_group_handler(int sock, int group_id) {

    char message[BUFFER_SIZE];
    int i, previous_group_id;

    for (i = 0; i < clients_array_pointer; i++)
        if (clients[i].socket == sock) {
            previous_group_id = clients[i].group_id;    // Store previous group id
            clients[i].group_id = group_id;            
        }
    
    // Response message to client    
    if (group_id == -1)
        sprintf(message, "[SERVER] You have left group %d.", previous_group_id);
    else
        sprintf(message, "[SERVER] You have joined group %d.", group_id);
    send(sock, message, strlen(message), 0);
}

// Returns a struct by its socket
struct Client get_client(int sock) {
    int i;
    for (i = 0; i < clients_array_pointer; i++)
        if (clients[i].socket == sock)
            return clients[i];
}

// Copy string (Avoid point)
char *copyString(char s[]) {
    char *s2 = (char *)malloc(20);
    strcpy(s2, s);
    return (char *)s2;
}

// Convert string into integer
int str2num(char *str) {
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

// Abstraction for error handling
void error_handler(int status, char error_message[50], char success_message[50])
{
    if (status < 0) {
        perror(error_message);
        exit(EXIT_FAILURE);
    }
    else
        printf("%s", success_message);
}