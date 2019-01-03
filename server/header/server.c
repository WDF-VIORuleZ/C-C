#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define _DST_PORT    58007
#define _SRC_PORT    57002

#define _IP      "127.0.0.1"


struct Client_Manager
{
    struct sockaddr_in  sock_clients[10];
    int                 int_clients[10];
    char                buffer[10][256];
    
};

typedef struct 
{
    struct Client_Manager cli_man;
    int sockfd, portno;
    socklen_t client_length;
    struct sockaddr_in server_address, client_address;
}Server;

bool add_sock_fd(Server *serv, int new_sock_fd)
{
    for (int i = 0; i < 10; i++)
    {
        if ((*serv).cli_man.int_clients[i] == 0)
        {
            (*serv).cli_man.int_clients[i] = new_sock_fd;
            return true;
        }
    }
    return false;
}

int initialize_Server(Server *dst, int portno, int max_clients)
{
    static Server server;// = malloc(sizeof(Server *));
    server.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *) &server, sizeof(server));

    server.server_address.sin_family      = AF_INET;
    server.server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server.server_address.sin_port        = htons(_SRC_PORT);

    // Bind Error: < 0
    int error = 0;

    // Conversion helper
    struct sockaddr_in server_adress = server.server_address;
    if (error = bind(server.sockfd, (struct sockaddr*)&server_adress, sizeof(server.server_address)) < 0)
        return error;
    else
        printf("Server successfully binded!");

    //(*dst) = (*server);
    dst = &server;
}

Server *create_Server(int portno, int max_clients)
{
    static Server server;
    bzero((char *)&server, sizeof(Server));

    server.server_address.sin_family      = AF_INET;
    server.server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    server.server_address.sin_port        = htons(portno);

    server.sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Bind Error: < 0
    int error = 0;

    // Conversion helper
    struct sockaddr_in server_adress = server.server_address;
    if ((error = bind(server.sockfd, (struct sockaddr*)&server_adress, sizeof(server.server_address))) < 0)
    {
        perror("ERROR IN CREATE_SERVER");
        return NULL;
    }
    
    return &server; 
}

int run(Server *serv)
{
    printf("In Run");

    struct sockaddr_in client_address;
    socklen_t          client_length;

    if(serv == NULL) return 0;

    // Listen for new connections
    int error = listen(serv->sockfd, sizeof(serv->cli_man.sock_clients)/sizeof(struct sockaddr_in));
    int new_socket_fd = accept(serv->sockfd, (struct sockaddr*)&client_address, &client_length);

    if (new_socket_fd >= 0)
    {
        if (add_sock_fd(serv, new_socket_fd))
        {
            printf("Added a socket!");
        }
        else perror("Got Error adding socket");
    }
    else 
    {
        perror("Error creating new socket file descriptor");
        return -1;
    }

    for(int i = 0; i < 10; i++)
    {
        printf("\tIN FOR LOOP : %d\n", i);
        char *buffer = serv->cli_man.buffer[i];
        bzero(buffer, 256);
        error = read(serv->cli_man.int_clients[i], buffer, 255);

        if (error < 0)
        {
            printf("Error Reading");
            return error;            
        }
    }

    for(int i = 0; i < 10; i++)
    {
        printf("%d : %s", i, serv->cli_man.buffer[i]);
    }

    for(int i = 0; i < 10; i++)
    {
        printf("%d : %d", i, serv->cli_man.int_clients[i]);
    }
}

void display_server_information(Server *server)
{
}