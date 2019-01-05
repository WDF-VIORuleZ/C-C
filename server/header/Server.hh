#ifndef __SERVER__
#define __SERVER__

#include "../../client/header/Client.hh"
#include "../../general_includes/tokens.h"
#include "../../file_handler/header/File_Handler.hh"

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/time.h>       // FD_SET, FD_ISSET, FD_ZERO macros 
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>         // memcpy
#include <locale.h>         // setlocale()

#include <string>
#include <vector>
#include <iostream>
#include <cctype>           // std::toupper
#include <thread>

using std::cerr;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

#define MAX_CLIENTS 10

class Server
{
    public:
        //CD - DS
        Server(int p_port);
        Server(int p_port, const string &p_log_root);
        ~Server();

        int  send_message(int sock_fd, const string& message);

        void read_client_buffers();
        void print_client_buffers();

        // Infos
        void display_server_stats();
        bool process_command(string command);
        bool display_error(const string &s, const int &e);

        // Main server Loop
        void run();

    private:
        int _socket_fd, _port_number;
        socklen_t client_length;
        struct sockaddr_in _server_address;

        std::vector<Client> _clients;

        // TODO: std::unique_ptr
        File_Handler *fh;

        // Client-regarding Methods
        void add_client(int client_sock_fd);
        void remove_client_dc(int client_sock_fd);
        bool client_exists(int client_sock_fd);
};

#endif // !__SERVER..#define __SERVER__




