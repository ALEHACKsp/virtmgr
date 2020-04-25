#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "vm.h"
#include "tui.h"
#include "gpu.h"
#include "utils.h"
#include "lib/inipp.h"

#define CONFIG_DIR "./"

void VmManager::CreateNew() 
{
    std::string title("New VM");
    std::string description("Create a new VM.");
    g_Tui->Text(title, description, "This wizard will guide you through creation of new VM.");

    std::string name = g_Tui->Input(title, description, "Enter VM name (ex. Windows 10 Pro x64): ");
    std::string ram = g_Tui->Input(title, description, "Amount of RAM in MB (ex. 1024 for 1GB of ram): ");

    std::string disk = g_Tui->Input(title, description, "Disk path (ex. /VMs/disk.img): ");
    if (!std::filesystem::exists(disk)) 
    {
        g_Tui->Text(title, description, "Disk file does not exist!");
        return;
    }
    std::string cddrive = g_Tui->Input(title, description, "CD path (ex. /VMs/shitdows.iso): ");
    if (!std::filesystem::exists(cddrive)) 
    {
        g_Tui->Text(title, description, "CD disk file does not exist!");
        return;
    }

    std::vector<std::string> gpumenu;
    std::vector<GpuManager::GpuInfo> gpuinfo = g_GpuManager->GetGPUs();
    for (auto& info : gpuinfo) 
    {
        gpumenu.push_back(info.name);
    }
    int response = g_Tui->Selection(title, "Select GPU to use in VM.", gpumenu);

    GpuManager::GpuInfo selectedgpu = gpuinfo.at(response - 1);

    inipp::Ini<char> ini;
    std::string random = g_Utils->RandomString(30) + ".ini"; 
    
    std::filebuf fb;
    fb.open(random.c_str(), std::ios::out);
    std::ostream os(&fb);

    ini.sections["VMSETTINGS"]["name"] = name;
    ini.sections["VMSETTINGS"]["ram"] = ram;
    ini.sections["VMSETTINGS"]["disk"] = disk;
    ini.sections["VMSETTINGS"]["cddrive"] = cddrive;
    ini.sections["VMSETTINGS"]["gpu"] = selectedgpu.id;

    ini.generate(os);

    fb.close();
}

VmManager::VmInfo VmManager::GetInfo(std::string path) 
{
    VmManager::VmInfo info;
    
    if (!std::filesystem::exists(path)) 
    {
        return info;
    }
    
    inipp::Ini<char> ini;
	std::ifstream is(path);
    ini.parse(is);

    info.name = ini.sections["VMSETTINGS"]["name"];
    info.ram = ini.sections["VMSETTINGS"]["ram"];
    info.disk = ini.sections["VMSETTINGS"]["disk"];
    info.cdrom = ini.sections["VMSETTINGS"]["cddrive"];
    info.gpuid = ini.sections["VMSETTINGS"]["gpu"];

    is.close();
    return info;
}

void VmManager::Start(VmManager::VmInfo info) 
{
    std::vector<std::string> menu;
    menu.push_back("Run VM");
    menu.push_back("Nope");
    
    int response = g_Tui->Selection("VM start", "Do you really want to start the VM?", menu);
    if (response != 1) 
        return;
    
    std::string command = "qemu-system-x86_64 -m " + info.ram + " -boot d -enable-kvm -cpu host,hv_time,kvm=off,hv_vendor_id=null,-hypervisor -smp " + "4" + " -net nic -net user -drive format=raw,file=" + info.disk + " -cdrom " + info.cdrom; // TODO: cpu cores 
    system(command.c_str());

    printf("Waiting 5 seconds...\n");
    std::this_thread::sleep_for(std::chrono::seconds(5));

    g_Tui->Text("VM shutdown", "Qemu has exited.", "VM was stopped. To start it again go to Quick start from main menu.");
}

std::vector<VmManager::VmInfo> VmManager::GetList() 
{
    std::vector<VmManager::VmInfo> list;

    for (const auto & entry : std::filesystem::directory_iterator(CONFIG_DIR)) 
    {
        std::string current = entry.path();

        if (current.find(".ini") != std::string::npos) 
        {
            list.push_back(GetInfo(current));
        }
    }

    return list;
}

void VmManager::Manage() 
{
    std::vector<std::string> menu;
    menu.push_back("Create new VM");
    menu.push_back("Edit exiting VM");
    menu.push_back("Go back");
    
    std::string title("VM Manager");
    std::string description("This section will help you manage your VMs.");
    int response = g_Tui->Selection(title, description, menu);

    // Create new VM
    if (response == 1) 
    {
        CreateNew();
    }
}