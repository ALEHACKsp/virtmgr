#ifndef _GPU_
#define _GPU_

#include <vector>
#include <string>

class GpuManager 
{
public:
    typedef struct _GpuInfo 
    {
        std::string name;
        std::string path;
        std::string id;
    } GpuInfo;
    
    void Manage();
    std::vector<GpuInfo> GetGPUs();

private:    
    void ListGPUs();
};

extern GpuManager* g_GpuManager;

#endif