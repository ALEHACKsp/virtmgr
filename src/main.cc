#include <unistd.h>
#include "tui.h"
#include "gpu.h"
#include "exec.h"
#include "utils.h"
#include "usb.h"
#include "disks.h"
#include "vm.h"

Tui* g_Tui;
GpuManager* g_GpuManager;
Exec* g_Exec;
Utils* g_Utils;
UsbManager* g_UsbManager;
DiskManager* g_DiskManager;
VmManager* g_VmManager;

bool running = true;

void MainMenu() 
{
    std::vector<std::string> menu;
    menu.push_back("Quick start");
    menu.push_back("Manage virtual machines");
    menu.push_back("Manage disks");
    menu.push_back("Manage GPUs");
    menu.push_back("About");
    menu.push_back("Exit");
    
    std::string title("Main Menu");
    std::string description("Welcome in virtmgr! This simple program was made to help you managing KVMs with GPU passthrough.");
    int response = g_Tui->Selection(title, description, menu);

    // Quick start
    if (response == 1) 
    {
        std::vector<std::string> vmmenu;
        std::vector<VmManager::VmInfo> list = g_VmManager->GetList();

        if (list.size() == 0) 
        {
            g_Tui->Text("Quick start", "Select VM you want to start.", "No VM configurations were found!");
            return;
        }

        for (auto& info : list) 
        {
            vmmenu.push_back(info.name);
        }

        int selectedvm = g_Tui->Selection("Quick start", "Select VM you want to start.", vmmenu);

        VmManager::VmInfo vm = list.at(selectedvm - 1);
        g_VmManager->Start(vm);
    }

    // Manage virtual machines
    if (response == 2) 
    {
        g_VmManager->Manage();
    }

    // Manage disks
    if (response == 3) 
    {
        g_DiskManager->Manage();
    }
    
    // Manage GPUs
    if (response == 4) 
    {
        g_GpuManager->Manage();
    }

    // About
    if (response == 5) 
    {
        char buffer[128];
        sprintf(buffer, "Build on: %s\nBuild in: %s", __DATE__, __TIME__);
        std::string text(buffer);

        g_Tui->Text("About", "There is some basics information about this build.", buffer);
    }

    // Exit
    if (response == 6) 
    {
        g_Tui->Exit();
        running = false;
        exit(0);
    }
}

bool CommandExists(std::string command) 
{
    std::string check = "which " + command + " > /dev/null 2>&1";
    return !system("which avconv > /dev/null 2>&1");
}

int main() 
{
    g_Tui = new Tui();
    g_GpuManager = new GpuManager();
    g_Exec = new Exec();
    g_Utils = new Utils();
    g_UsbManager = new UsbManager();
    g_DiskManager = new DiskManager();
    g_VmManager = new VmManager();

    if (getuid()) 
    {
        printf("Not running as root user! Exiting.\n");
        return -1;
    }

    if (!CommandExists("qemu-system-x86_64")) 
    {
        printf("Qemu does not seem to be installed (or qemu-system-x86_64 was not found in path). Exiting.\n");
    }

    while (running) 
    {
        MainMenu();
    }
}