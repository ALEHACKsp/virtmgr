#ifndef _DISKS_
#define _DISKS_

#include <vector>
#include <string>

class DiskManager 
{
public:
    typedef struct _UsbInfo 
    {
        std::string name;
        std::string id;
    } UsbInfo;
    
    void Manage();
private:  
    void CreateNew();
    void Edit();
};

extern DiskManager* g_DiskManager;

#endif