#ifndef CC_MENU
#define CC_MENU

#include <olcPixelGameEngine.h>

class MenuContent
{
protected:
    int posX; // within menu box
    int posY; // within menu box
public:
    virtual void drawYourself(olc::PixelGameEngine* pge, olc::vi2d menuOffset) = 0;

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

    void drawYourself(olc::PixelGameEngine* pge, olc::vi2d menuOffset) override;
};

class PressEntry : public MenuContent
{
private:
    std::string key1;
    std::string key2;
    std::string action;
    uint32_t fontSize;

    void pressEntry(olc::PixelGameEngine* pge, const olc::vi2d pos, const olc::vi2d menuOffset, const std::string& key, const std::string& action, const uint32_t fontSize = 1);
    void pressEntry(olc::PixelGameEngine* pge, const olc::vi2d pos, const olc::vi2d menuOffset, const std::string& key1, const std::string& key2, const std::string& action, const uint32_t fontSize = 1);
public:
    PressEntry();
    explicit PressEntry(int posX, int posY, std::string key1, std::string key2, std::string action, uint32_t fontSize);
    explicit PressEntry(int posX, int posY, std::string key, std::string action, uint32_t fontSize);
    ~PressEntry();

    void drawYourself(olc::PixelGameEngine* pge, olc::vi2d menuOffset) override;
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

    void moveMenu(int deltaX, int deltaY);

    void addContentToMenu(std::shared_ptr<MenuContent> content);
    void drawYourself(olc::PixelGameEngine* pge);
};

#endif
