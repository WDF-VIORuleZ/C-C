#ifndef __FILE_HANDLER__
#define __FILE_HANDLER__


#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

using std::string;
using std::cout;
using std::cerr;
using std::endl;

class File_Handler
{
    public:
        // CS
        File_Handler(const fs::path &log_path);

        fs::path _log_path;

        const fs::path _log_clients = "/log/clients";
        const fs::path _log_server  = "/log/server";


        const void log(const string &s);
        const void log(int client_fd, const string &buf);

    private:
        const void mkfs(fs::path fs_root);
        
};

#endif // !__FILE_HANDLER__
