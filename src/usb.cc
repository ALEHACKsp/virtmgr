#include <vector> // retarded vscode needs it to be everywhere
#include <string>
#include "usb.h"
#include "exec.h"
#include "utils.h"

std::vector<UsbManager::UsbInfo> UsbManager::GetUSBs() 
{
    std::vector<UsbManager::UsbInfo> list;

    // example
    // Bus 003 Device 003: ID 0951:16d2 Kingston Technology HyperX Alloy FPS Pro Mechanical Gaming Keyboard
    std::string output = g_Exec->Run("lsusb");

    std::string newline("\n");
    std::vector<std::string> lines = g_Utils->Split(output, newline);

    for (auto& line: lines) 
    {
        try 
        {
            UsbManager::UsbInfo info;
            
            int idpos = line.find("ID");
            int idend = line.find(" ", idpos + 4);
            std::string id = line.substr(idpos + 3, idend - idpos - 3);

            std::string name = line.substr(idend + 1, line.length() - idend - 1);

            info.id = id;
            info.name = name;

            list.push_back(info);
        } catch (std::exception error)
        {
            // nothing
            // probably blank line or some random shit
        }      
    }

    return list;
}