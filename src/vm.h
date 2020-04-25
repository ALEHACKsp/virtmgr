#ifndef _VM_
#define _VM_

#include <vector>
#include <string>

class VmManager 
{
public:
    typedef struct _VmInfo 
    {
        std::string name;
        std::string disk;
        std::string cdrom;
        std::string gpuid;
        std::string ram;
    } VmInfo;
    
    void Manage();
    VmInfo GetInfo(std::string path);
    void Start(VmInfo info);
    std::vector<VmInfo> GetList();
private:  
    void CreateNew();
    void Edit();
};

extern VmManager* g_VmManager;

#endif