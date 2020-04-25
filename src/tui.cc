#include <iostream>
#include <vector>
#include <string>
#include "tui.h"

void Tui::ClearScreen() 
{
    printf("\033[2J"); // clear screen
    printf("\033[1;1H"); // move cursor to the top left corner
}

void Tui::SetColor(Color color) 
{
    printf("\x1B[%im", color);
}

void Tui::PrintStatic() 
{
    SetColor(Color::Red);
    printf("       _     __                \n");
    printf(" _  __(_)___/ /___ _  ___ _____\n");
    printf("| |/ / / __/ __/  ' \\/ _ `/ __/\n");
    printf("|___/_/_/  \\__/_/_/_/\\_, /_/   \n");
    printf("                    /___/      \n");
    printf("\n");
    
    SetColor(Color::Green);
    printf("Copyright (c) 2020 Samuel Tulach - All rights reserved\n"); // hehe
    //printf("Build on %s in %s\n", __DATE__, __TIME__);
    printf("tulach.cc/virtmgr | github.com/SamuelTulach/TODOHERE\n");
    printf("\n");
    SetColor(Color::Reset);
}

void Tui::PrintAll(std::string title, std::string description, std::vector<Tui::Element> elements) 
{   
    // clear the screen of the mess
    ClearScreen();

    // flex with ascii title
    PrintStatic();

    SetColor(Color::Bold);
    SetColor(Color::Underline);
    printf("%s\n", title.c_str());
    
    SetColor(Color::Reset);
    printf("%s\n", description.c_str());
    printf("\n");

    // loop all the elements
    for (auto& element: elements) 
    {
        if (element.type == Tui::ElementType::Label) 
        {
            LabelElement* label = (LabelElement*)element.ptr;
            printf("%s", label->text.c_str());
        }
    }
}

void Tui::Render(std::string title, std::string description) 
{
    PrintAll(title, description, _elements);
}

void Tui::Exit() 
{
    SetColor(Color::Reset);
    ClearScreen();
}

int Tui::Selection(std::string title, std::string description, std::vector<std::string> items) 
{
    _elements.clear();
    
    int total = 1;
    for (auto& item: items) 
    {
        Tui::Element element;
        element.type = Tui::ElementType::Label;

        LabelElement* label = new LabelElement();
        label->text = std::to_string(total) + ".) " + item + "\n";

        element.ptr = label;

        _elements.push_back(element);
        total++;
    }

    Tui::Element element;
    element.type = Tui::ElementType::Label;

    LabelElement* label = new LabelElement();
    label->text = "\nEnter option: ";
    
    element.ptr = label;
    
    _elements.push_back(element);

    Render(title, description);

    std::string input;
    getline(std::cin, input);

    try 
    {
        int response = std::stoi(input);

        if (response > 0 && response <= items.size()) 
        {
            return response;
        }
    } catch (std::invalid_argument error)
    {
        // nope
    }
    
    // invalid response, try again
    return Selection(title, description, items);
}

void Tui::Text(std::string title, std::string description, std::string text) 
{
    _elements.clear();

    Tui::Element element;
    element.type = Tui::ElementType::Label;

    LabelElement* label = new LabelElement();
    label->text = text + "\n\nPress ENTER to continue\n";

    element.ptr = label;

    _elements.push_back(element);

    Render(title, description);

    getchar();
}

std::string Tui::Input(std::string title, std::string description, std::string text) 
{
    _elements.clear();

    Tui::Element element;
    element.type = Tui::ElementType::Label;

    LabelElement* label = new LabelElement();
    label->text = text;

    element.ptr = label;

    _elements.push_back(element);

    Render(title, description);

    std::string input;
    getline(std::cin, input);

    return input;
}