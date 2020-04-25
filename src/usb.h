#ifndef _USB_
#define _USB_

#include <vector>
#include <string>

class UsbManager 
{
public:
    typedef struct _UsbInfo 
    {
        std::string name;
        std::string id;
    } UsbInfo;
    
    std::vector<UsbInfo> GetUSBs();
private:  
    void ListGPUs();
};

extern UsbManager* g_UsbManager;

#endif