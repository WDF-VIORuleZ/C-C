#include "../header/Client.hh"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "de_DE.UTF.8");
    Client *client;

    if (argc == 2)
    {
        client = new Client(string(argv[1]), atoi(argv[2]));
        cout << LOG::OK << "Created Client" << endl;
    }
    else 
    {
        client = new Client("127.0.0.1", 31337);
    }

    client->connect_to_server();
    client->run();
    client->disconnect_from_server();
    
    return 0;
}