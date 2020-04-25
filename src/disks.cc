#include <string>
#include <filesystem>
#include "disks.h"
#include "tui.h"
#include "exec.h"

void DiskManager::CreateNew() 
{
    std::string title("New disk");
    std::string description("Create a new disk for use in VMs.");
    g_Tui->Text(title, description, "This wizard will guide you through creation of new virtual disk drive.");

    std::string path = g_Tui->Input(title, description, "Disk filename: ");

    std::vector<std::string> formatmenu;
    formatmenu.push_back("RAW");
    formatmenu.push_back("Qcow2 (not supported atm)"); // TODO: make support
    int format = g_Tui->Selection(title, description, formatmenu);
    std::string formattext;
    if (format == 1) 
    {
        formattext += "raw";
    } else if (format == 2) // useless but for future ya know?
    {
        formattext += "qcow2";
    }

    std::string size = g_Tui->Input(title, description, "Disk size (ex. 20G): ");

    std::string command = "qemu-img create -f " + formattext + " " + path + " " + size;
    std::string output = g_Exec->Run(command.c_str());

    g_Tui->Text(title, description, output);
}

void DiskManager::Edit() 
{
    std::string title("Edit disk");
    std::string description("Edit existing disk size and other options.");
    g_Tui->Text(title, description, "This wizard will guide you through editing of virtual disk drive.");

    std::string path = g_Tui->Input(title, description, "Disk filename: ");

    if (!std::filesystem::exists(path)) 
    {
        g_Tui->Text(title, description, "Disk file does not exist!");
        return;
    }

    std::string size = g_Tui->Input(title, description, "Enter new size (ex. 20G): ");

    std::string command = "qemu-img resize " + path + " " + size;
    std::string output = g_Exec->Run(command.c_str());

    g_Tui->Text(title, description, output);
}

void DiskManager::Manage() 
{
    std::vector<std::string> menu;
    menu.push_back("Create new disk");
    menu.push_back("Edit exiting disk");
    menu.push_back("Go back");
    
    std::string title("Disk Manager");
    std::string description("This section will help you manage your disks for use in VMs.");
    int response = g_Tui->Selection(title, description, menu);

    // Create new disk
    if (response == 1) 
    {
        CreateNew();
    }

    // Edit exiting disk
    if (response == 2) 
    {
        Edit();
    }
}