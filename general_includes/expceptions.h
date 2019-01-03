#ifndef __CUTSOM_EXCEPT__
#define __CUTSOM_EXCEPT__

#include <exception>


class Syntax_Except : public std::exception
{
    public:
        inline Syntax_Except(int _param) : param (_param) {}
        const int &get_param() noexcept {return param;}

    private:
        int param;
};

#endif // !__CUTSOM_EXCEPT__