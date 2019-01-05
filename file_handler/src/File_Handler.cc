#include "../header/File_Handler.hh"


File_Handler::File_Handler(const fs::path &log_path)
{

    std::ofstream ofs(log_path.string());

    if (fs::exists(log_path) && ofs.is_open() && ofs.good() && !ofs.bad())
    {
        cout << "Set Gloabl LogPath to " << _log_path << endl;
        _log_path = log_path;
    }
}

const void File_Handler::mkfs(fs::path fs_root)
{
    if (fs::exists(fs_root))
    {
        cout << "Root Folder already exists. Skipping FS creation" << endl;       
    }
    else
    {
        fs::create_directories(fs_root += _log_clients);
        fs::create_directory(fs_root += _log_server);

        log("Created FS");
    }
}

const void File_Handler::log(const string &s)
{
    std::ofstream ofs(_log_server.string());

    if (fs::exists(_log_server) && ofs.good())
    {
        // time for logging
        std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(tp);
        ofs << "[" << std::ctime(&t) << "]" << s << endl;
    }
    else
    {
        cerr << "Could not open Log" << endl;
    } 
}

const void File_Handler::log(int client_fd, const string &buf)
{
    string _client_path = _log_clients.string() + std::to_string(client_fd) + ".dat";
    std::ofstream ofs(_client_path);

    if (fs::exists(_client_path) && ofs.good() && !ofs.bad())
    {
        std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(tp);
        ofs << "[" << std::ctime(&t) << "]" << buf << endl;
    }
    else
    {
        cerr << "Could not open Log" << endl;
    }
}