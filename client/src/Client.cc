#include "../header/Client.hh"

Client::Client(int client_socket_fd, int server_port, struct sockaddr_in server_address)
{
    _socket_fd      = client_socket_fd;
    _server_port    = server_port;
    _server_address = server_address;

    bzero((char *) &_server_address, sizeof(_server_address));
    _server_address.sin_family = AF_INET;
    _server_address.sin_port   = htons(server_port);

}

Client::Client(string server_ip, int server_port)
{
    _socket_fd      = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    _server_port    = server_port;

    if (inet_pton(AF_INET, server_ip.c_str(), &_server_address.sin_addr) <=0 )  
    { 
        cout << LOG::ERR << "Could not convert hostname " << server_ip << " to binary_data. Exit" << endl;
        exit(-1);
    }
    else
    {
        cout << LOG::DBG << LOG::OK << "Set _server_address.sin_adrr correctly" << endl;
    } 

    bzero((char *)&_server_address, sizeof(_server_address));
    _server_address.sin_family       = AF_INET;
    _server_address.sin_port         = htons(server_port);
    //_server_address.sin_addr.s_addr  = atoi(gethostbyname(server_ip.c_str())->h_addr);
}

void Client::connect_to_server()
{
    if (connect(_socket_fd,(struct sockaddr *) &_server_address,sizeof(_server_address)) < 0) 
    {
        cout << LOG::ERR << "Could not connect to Server: " << _server_address.sin_addr.s_addr << endl;
    }
    else
    {
        cout << LOG::OK << "Connected to C&C" << endl;
        _is_connected = true;
    }
}

void Client::disconnect_from_server()
{
    if (close(_socket_fd) < 0)
    {
        cout << LOG::OK << "Closed Socket connection to Server" << endl;
        _is_connected = false;
    }
    else
    {
        cout << LOG::ERR << "Error closing connection" << endl; 
    }
    
}

void Client::send_message(std::string message)
{
    if (_is_connected)
    {
        if (write(_socket_fd, message.c_str(), message.length()) < 0)
        {
            cout << LOG::ERR << "Error writing message [" << message.length() << "] << to server" << endl;
        }
        else
        {
            cout << LOG::OK << "Sent Message: " << message << endl;
        }
    }
    else
    {
        cout << LOG::WRN << "Did not write to server (not connected)" << endl;
    }
}

void Client::receive_message()
{

    if (_is_connected)
    {
        bzero(receive_buffer, 256);
        if (read(_socket_fd, receive_buffer, 255) < 0)
        {
            cout << LOG::ERR << "Error receiving message" << endl;
        }
        else
        {
            cout << LOG::OK << "Received Message: \n" << receive_buffer << endl;
        }
    }
    else
    {
        cout << LOG::ERR << "Could not read from server (not connected)" << endl;
    }
}

void Client::run()
{
    bool status = true;
    
    if (_is_connected)
    {
        do
        {
            // TODO: Listen Thread
            cout << LOG::LOG << "Waiting for message from Server" << endl;
            receive_message();
            sleep(1);
        }
        while(status);
    }
    else
    {
        cout << LOG::ERR << "Not connected" << endl; 
    }
    
}