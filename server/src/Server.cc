#include "../header/Server.hh"
//#include "../header/LOGGING.h"

Server::Server(int p_port)
{
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    _port_number = p_port;

    _server_address.sin_family      = AF_INET;
    _server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    _server_address.sin_port        = htons(_port_number);
    int reuseaddr                   = 1;

    if (setsockopt(_socket_fd,SOL_SOCKET,SO_REUSEADDR,&reuseaddr,sizeof(reuseaddr)) == -1) 
    {
        cerr << LOG::ERR << "Error Seeting SpockOPT" << endl;
    }
    else
    {
        cout << LOG::OK << "Set Sockopt: reuseaddr" << endl;
    }

    // Conversion helper
    struct sockaddr_in server_adress = _server_address;
    if (bind(_socket_fd, (struct sockaddr*)&server_adress, sizeof(_server_address)) < 0)
    {
        cerr << LOG::ERR << "Error binding Server to port: " << _port_number << " @ ADRR: " << _server_address.sin_addr.s_addr << endl;
    }    
    else
    {
        cout << LOG::OK << "Successfully Created Server on port " << _port_number << " @ ADRR " << _server_address.sin_addr.s_addr << endl; 
    }   
}

void Server::add_client(int client_sock_fd)
{
    Client tmp(client_sock_fd, _port_number, (struct sockaddr_in)_server_address);

    if (!client_exists(client_sock_fd) && _clients.size() < _clients.max_size())
    {
        _clients.push_back(tmp);
        cout << LOG::OK << " Added Client " << endl;
    }
}

void Server::remove_client_dc(int client_sock_fd)
{
    if (client_exists(client_sock_fd))
    {
        for(int i = 0; i < _clients.size(); i++)
        {
            if(_clients[i]._socket_fd == client_sock_fd)
            {
                _clients.erase(_clients.begin() + i);
            }
        }
    }
}

bool Server::client_exists(int client_sock_fd)
{
    for (auto i : _clients)
    {
        if (i._socket_fd == client_sock_fd)
            return true;  
    }
    return false;      
}

void Server::display_server_stats()
{
    system("clear");
    cout << string(50, '-') << endl;

    cout << "|\n|\tSERVER_IP: " << _server_address.sin_addr.s_addr 
         << "\n|\tPort : "<< _port_number
         << "\n|\tClients_con: " << _clients.size() << endl;

    cout << string(50, '-') << endl;
}

void Server::run()
{
    bool status = true;
    string command_input;

    while (true)
    {
        do
        {
            command_input = "";
            std::cin.clear();

            cout << "CMD> ";
            std::getline(std::cin, command_input);

            cout << LOG::DBG << "Got CLI_IN = " << command_input << endl;
 
            if(!cin.fail())
            {
                status = process_command(command_input);
                if(_clients.size() > 0)
                {
                    for (int i = 0; i < _clients.size(); i++)
                    {
                        send_message(_clients[i]._socket_fd, "MSG_C&C");
                    }
                    read_client_buffers();
                    print_client_buffers();
                }
            }
            else
            {
                cout << LOG::ERR << "\nInvalid command. Type HELP for list of valid commands" << endl;
            }
        }
        while(status);
        
        //int error = listen(_socket_fd, 10);
       
       /*
            Checking for remaining open ports
       */
        for(size_t i = 0; i < MAX_CLIENTS - _clients.size(); i++)
        {
            if (!listen(_socket_fd, MAX_CLIENTS))
            {
                cout << LOG::LOG << "Listeing for Clients..." << endl;

                struct sockaddr_in client_address;
                socklen_t          client_length;

                int new_socket_fd = accept(_socket_fd, (struct sockaddr*)&client_address, &client_length);

                if (new_socket_fd > 0)
                {
                    add_client(new_socket_fd);
                    cout << "Added new Client: " << new_socket_fd << endl;
                }
                else
                {
                    cerr << "Error Creating socket_fd. Mission Abort" << endl;
                    exit(-1);
                }
            }
            else
            {
                cout << LOG::ERR << "Error Listeing" << endl;
            }
        }
        // Reading clients messages to buffer
    }
}

void Server::read_client_buffers()
{
    // TODO: Implement timeout
    cout << LOG::LOG << _clients.size() << " Clients connected\n" <<
            LOG::LOG << "Reading clients buffers ..." << endl;

    for (int i = 0; i < _clients.size(); i++)
    {
        bzero(&_clients[i].receive_buffer, 256);
        int status = read(_clients[i]._socket_fd, _clients[i].receive_buffer, 255);

        if (status < 0)
        {
            cerr << "Error reading from socket." << endl;
            exit(-1);
        }
    }

}

void Server::print_client_buffers()
{
    if (_clients.size() > 0)
    {
        cout << "Printing client buffers ..." << endl;
        for (int i = 0; i < _clients.size(); i++)
        {
            cout << "CLI[" << _clients[i]._socket_fd << "] : " << _clients[i].receive_buffer << endl;
        }
    }
    else
    {
        cout << LOG::WRN << "No clients are connected" << endl;
    }    
}

bool Server::process_command(string command)
{
    hlp::tokens toke = hlp::parse_tokens(command);
    for (auto &i : toke)
        cout << LOG::DBG << i << endl;

    bool rtn = true;

    switch(toke.size())
    {
        case 1:
            if (toke[0] == "EXIT")
            {
                cout << LOG::OK << "Exiting C&C Interface" << endl;
                rtn = false;
                break;
            }
            else if (toke[0] == "HELP")
            {
                cout << LOG::LOG << "Options: \n"
                                 << "\t[1] (EXIT, HELP [this*] )\n"
                                 << "\t[2] LIST [BUFFER, CLIENTS, STATS]\n"
                                 << "\t[3] (/)\n"
                                 << "\t[4] SEND MESSAGE ALL \"_MSG_\"\n" 
                                 << endl; 
                break;
            }
            else 
            {
                rtn = false;
                break;
            }

        case 2:
            if (toke[0] == "LIST")
            {
                if (toke[1] == "BUFFER")
                {
                    cout << LOG::OK << " LIST BUFFER" << endl;
                    print_client_buffers();
                    break;
                }
                else if (toke[1] == "CLIENTS")
                {
                    cout << LOG::OK << _clients.size() << " Clients are connected: " << endl;
                    for (int i = 0; i < _clients.size(); i++)
                    {
                        cout << "\t " << _clients.at(i)._socket_fd << endl;  
                    } 
                    break;
                }
                else if (toke[1] == "STATS")
                {
                    cout << LOG::OK << "Showing Server Status: "<< endl;
                    display_server_stats();
                    break;
                }
                else
                {
                    rtn = false;
                    break;
                }
            }
            else
            {
                rtn = false;
                break;
            }

        case 4:
            // TODO: Implement try-catch
            if (toke[0] == "SEND")
            {
                if (toke[1] == "MESSAGE")
                {
                    if (toke[2] == "ALL")
                    {
                        if (toke[3] != "")
                        {
                            for (size_t i = 0; i < _clients.size(); i++)
                            {
                                if (send_message(_clients[i]._socket_fd, toke[3]) != -1)
                                {
                                    cout << LOG::OK << "Sent message to " << _clients[i]._socket_fd << endl;
                                    return true;
                                }
                            }
                        }
                        else
                        {
                            rtn = false;
                            break;
                        }
                    }
                    else
                    {
                        rtn = false;
                        break;
                    }
                }    
                else
                {
                    rtn = false;
                    break;
                }
            }
            else
            {
                rtn = false;
                break;
            }

        default:
            cout << LOG::ERR << "Invalid Command(s). Please Retry ..." << endl; 
            rtn = false;
            break;
    }
    return rtn;
}

int Server::send_message(int sock_fd, const string& message)
{
    cout << LOG::LOG << "Starting Message transmission ..." << endl;
    return write(sock_fd, message.c_str(), message.size() + 1);
}

bool Server::display_error(const string &s, const int &e)
{
    cout << LOG::ERR << "Invalid Syntax [" << e << "] (" << s << ")" << endl;
    return false;
}