#ifndef _TUI_
#define _TUI_

#include <vector>
#include <string>

class Tui 
{
private:
    enum Color
    {
        Reset,
        Bold,
        Dim,
        Italic,
        Underline,
        Black = 30,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        BackgroundBlack = 40,
        BackgroundRed,
        BackgroundGreen,
        BackgroundYellow,
        BackgroundBlue,
        BackgroundPurple,
        BackgroundCyan,
        BackgroundWhite,
        BrightBlack = 90,
        BrightRed,
        BrightGreen,
        BrightYellow,
        BrightBlue,
        BrightMagenta,
        BrightCyan,
        BrightWhite
    };

    enum ElementType 
    {
        Label
    };

    typedef struct _LabelElement 
    {
        std::string text;
    } LabelElement;
    
    typedef struct _Element 
    {
        ElementType type;
        void* ptr;
    } Element;

    std::vector<Element> _elements;

    void PrintStatic();
    void PrintAll(std::string title, std::string description, std::vector<Element> elements);
    void SetColor(Color color);
public:
    void ClearScreen();
    void Render(std::string title, std::string description);
    int Selection(std::string title, std::string description, std::vector<std::string> items);
    void Text(std::string title, std::string description, std::string text);
    std::string Input(std::string title, std::string description, std::string text);
    void Exit();
};

extern Tui* g_Tui;

#endif