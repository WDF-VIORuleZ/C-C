#ifndef __CLIENT__
#define __CLIENT__

#include "../../general_includes/logging.h"
//#include "../../server/header/Server.hh"

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netdb.h>
#include <locale.h>         // setlocale()

#include <iostream>
#include <string>


using std::cout;
using std::cerr;
using std::endl;
using std::string;

class Client
{
    public:
        Client(int client_socket_fd, int server_port, struct sockaddr_in server_address);
        Client(string server_ip, int server_port);

        int _socket_fd, _server_port;
        bool _is_connected = false;
        struct sockaddr_in _server_address;
        char receive_buffer[256];

        void connect_to_server();
        void disconnect_from_server();

        void send_message(std::string message);
        void receive_message();

        void run();
};

#endif // !__CLIENT__
