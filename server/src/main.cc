#include "../header/Server.hh"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "de_DE.UTF.8");
    Server *s;

    if (argc == 2)
    {
        s =  new Server(std::stoi(string(argv[1])));
        cout << LOG::LOG << "CUSTOM PORT" << endl;
    }
    else if (argc == 3)
    {
        s = new Server(std::stoi(string(argv[1])), string(argv[2]));
        cout << LOG::LOG << "CUSTOM PORT\n"
             << LOG::LOG << "CUSTOM FS_ROOT" << endl;
    }
    else
    {
        s = new Server(31337);
        cout << LOG::LOG << "STD_PORT [31337]"
             << LOG::LOG << "FS_ROOT  [" << fs::current_path().string() << "]" << endl;
    }
    sleep(2);

    s->display_server_stats();
    s->run();
    return 0;
}