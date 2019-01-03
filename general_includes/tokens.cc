#include "tokens.h"

    hlp::tokens hlp::parse_tokens(const std::string &raw_lower)
    {
        hlp::tokens _t;
        std::string _upper, token;

        for(auto &c : raw_lower)
        {
            if (c == ' ')
            {
                _upper += ' ';
            }
            else
            {
                _upper += std::toupper(c);
            }        
        }
        std::istringstream iss(_upper);

        while(std::getline(iss, token, ' '))
        {
            _t.push_back(token);
        }
        return _t;
    }