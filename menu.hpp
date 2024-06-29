#ifndef CC_MENU
#define CC_MENU

#include <olcPixelGameEngine.h>

class MenuContent
{
protected:
    int posX; // within menu box
    int posY; // within menu box
public:
    virtual void drawYourself(olc::PixelGameEngine* pge, olc::vi2d offset) = 0;

    MenuContent();
    explicit MenuContent(int posX, int posY);
    
    virtual ~MenuContent();
};

class Text : public MenuContent
{
private:
    std::string str;
    uint32_t fontSize;

public:
    Text();
    explicit Text(int posX, int posY, std::string str, uint32_t fontSize);
    ~Text();

    void drawYourself(olc::PixelGameEngine* pge, olc::vi2d offset) override;
};

class PressEntry : public MenuContent
{
private:
    std::string key1;
    std::string key2;
    std::string action;
    uint32_t fontSize;

    void pressEntry1(olc::PixelGameEngine* pge, const olc::vi2d pos, const olc::vi2d offset);
    void pressEntry2(olc::PixelGameEngine* pge, const olc::vi2d pos, const olc::vi2d offset);
public:
    PressEntry();
    explicit PressEntry(int posX, int posY, std::string key1, std::string key2, std::string action, uint32_t fontSize);
    explicit PressEntry(int posX, int posY, std::string key, std::string action, uint32_t fontSize);
    ~PressEntry();

    void drawYourself(olc::PixelGameEngine* pge, olc::vi2d offset) override;
};

class Menu
{
private:
    std::vector<std::shared_ptr<MenuContent>> contents;

    int posX;
    int posY;
    unsigned short windowSizeX;
    unsigned short windowSizeY;
    
public:
    Menu();
    explicit Menu(int posX, int posY, unsigned short windowSizeX, unsigned short windowSizeY);
    ~Menu();

    int getPosX();
    int getPosY();

    void moveMenu(int deltaX, int deltaY);
    void resetPos();
    void addContentToMenu(std::shared_ptr<MenuContent> content);
    void drawYourself(olc::PixelGameEngine* pge);
};

#endif
