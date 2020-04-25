#ifndef _UTILS_
#define _UTILS_

#include <vector>
#include <string>

class Utils
{
public:
    std::vector<std::string> Split(std::string& str, std::string& delimiter);
    std::string RandomString(size_t length);
};

extern Utils* g_Utils;

#endif