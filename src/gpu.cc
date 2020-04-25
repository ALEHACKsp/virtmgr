#include "gpu.h"
#include "tui.h"
#include "exec.h"
#include "utils.h"

std::vector<GpuManager::GpuInfo> GpuManager::GetGPUs() 
{
    std::vector<GpuManager::GpuInfo> list;
    
    // example: 
    // 00:01.0/01:00.0 VGA compatible controller [0300]: NVIDIA Corporation GP106 [GeForce GTX 1060 3GB] [10de:1c02] (rev a1)
    std::string output = g_Exec->Run("lspci -k -PP -nn | grep -i \"VGA compatible controller\"");

    std::string newline("\n");
    std::vector<std::string> lines = g_Utils->Split(output, newline);

    for (auto& line: lines) 
    {
        try 
        {
            GpuManager::GpuInfo info;
            
            int pathpos = line.find(" ");
            std::string path = line.substr(0, pathpos);
            
            int vgapos = line.find("VGA compatible controller");
            int namepos = line.find("]:", vgapos);
            int nameend = line.find("[", namepos);
            std::string name = line.substr(namepos + 3, nameend - namepos - 3); // oh no

            int idpos = line.find_last_of("[");
            int idend = line.find_last_of("]");
            std::string id = line.substr(idpos + 1, idend - idpos - 1); // it's trash

            info.id = id;
            info.name = name;
            info.path = path;

            list.push_back(info);
        } catch (std::exception error)
        {
            // nothing
            // probably blank line or some random shit
        }      
    }

    return list;
}

void GpuManager::ListGPUs() 
{
    std::vector<GpuManager::GpuInfo> list = GetGPUs();

    std::string title("GPU list");
    std::string description("You can see all GPUs that are now attached to the system.");

    if (list.size() == 0) 
    {
        g_Tui->Text(title, description, "No GPUs found.");
        return;
    }

    std::string finaltext;
    for (auto& info: list) 
    {
        finaltext += "ID: \t" + info.id + "\n";
        finaltext += "Name: \t" + info.name + "\n";
        finaltext += "Path: \t" + info.path + "\n";
    }

    g_Tui->Text(title, description, finaltext);
}

void GpuManager::Manage() 
{
    std::vector<std::string> menu;
    menu.push_back("List available GPUs");
    menu.push_back("See GPU setup instructions");
    menu.push_back("Go back");
    
    std::string title("GPU Manager");
    std::string description("This section will help you manage your GPUs for use in VMs.");
    int response = g_Tui->Selection(title, description, menu);

    // List available GPUs
    if (response == 1) 
    {
        ListGPUs();
    }

    // See GPU setup instructions
    if (response == 2) 
    {
        g_Tui->Text("GPU setup", "These are some guides to help you get started.", "Arch Linux:\nhttps://wiki.archlinux.org/index.php/PCI_passthrough_via_OVMF\n\nUbuntu/Debian:\nhttps://wiki.debian.org/VGAPassthrough");
    }
}