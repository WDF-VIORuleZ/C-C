#include <string>


namespace LOG
{
    static const std::string ERR =     "\033[31m[ERR]\033[0m ";
    static const std::string WRN =     "\033[33m[WRN]\033[0m ";
    static const std::string OK  =     "\033[32m[OK] \033[0m ";
    static const std::string LOG =  "\033[37;40m[LOG]\033[0m ";
    static const std::string DBG = "\033[11;106m[DBG]\033[0m ";
} // LOG

// Foreground only
enum class CLR
{
    RED = 31,
    GRN,
    YLW,
    BLU,
    DFT = 39,
};

/*
    std::istringstream iss([command](const string& val) -> std::string
    {
        static string _tmp = "";
        for (const char &c : command) 
            _tmp += std::toupper(c);
        return _tmp;
    });
    */
/*

class CLR_MOD
{
    CLR _clr;

    public:
        CLR_MOD(CLR p_clr) : _clr(p_clr) {}
        friend std::ostream& operator<<(std::ostream& os, const CLR_MOD& mod)
        {
            return os << "\033[" << mod._clr;
        }
*/