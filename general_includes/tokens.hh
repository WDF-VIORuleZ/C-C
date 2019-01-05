#include <ostream>
#include <vector>
#include <sstream>              // istringstream
#include <iostream>             
#include <string>

using std::cout;
using std::endl;
using std::vector;

namespace hlp
{
    typedef std::vector<std::string> tokens;
    tokens parse_tokens(const std::string &raw_lower);
}
