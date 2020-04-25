#ifndef _EXEC_
#define _EXEC_

#include <string>

class Exec
{
public:
    std::string Run(const char* cmd);
};

extern Exec* g_Exec;

#endif