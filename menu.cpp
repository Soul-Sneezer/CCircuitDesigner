#include "menu.hpp"

int Menu::getPosX()
{
    return this->posX;
}

int Menu::getPosY()
{
    return this->posY;
}

MenuContent::MenuContent()
{
    this->posX = 0;
    this->posY = 0;
}

MenuContent::MenuContent(int posX, int posY)
{
    this->posX = posX;
    this->posY = posY;
}

MenuContent::~MenuContent()
{
}

Text::Text()
{
    this->str = "";
    this->fontSize = 1;
}

Text::Text(int posX, int posY, const std::string str, uint32_t fontSize) : MenuContent(posX, posY) 
{
    this->str = str;
    this->fontSize = fontSize;
}

Text::~Text()
{
}

PressEntry::PressEntry(int posX, int posY, const std::string key, const std::string action, uint32_t fontSize) : MenuContent(posX, posY)
{
    this->posX = posX;
    this->posY = posY;

    this->key1 = key;
    this->action = action;
    this->fontSize = fontSize;
}

PressEntry::PressEntry(int posX, int posY, const std::string key1, const std::string key2, const std::string action, uint32_t fontSize) : MenuContent(posX, posY)
{
    this->posX = posX;
    this->posY = posY;

    this->key1 = key1;
    this->key2 = key2;
    this->action = action;
    this->fontSize = fontSize;
}

PressEntry::PressEntry()
{
    this->key1 = "";
    this->key2 = "";
    this->action = "";
    fontSize = 1;
}

PressEntry::~PressEntry()
{
}

void PressEntry::pressEntry1(olc::PixelGameEngine* pge, const olc::vi2d pos, olc::vi2d offset)
{
    pge->DrawString(pos.x + offset.x                , pos.y + offset.y , "Press ", olc::WHITE, this->fontSize);
    pge->DrawString(pos.x + offset.x + 48 * fontSize, pos.y + offset.y , this->key1,      olc::GREEN, this->fontSize);
    pge->DrawString(pos.x + offset.x + (48 + 8 * key1.size()) * fontSize, pos.y + offset.y , " to ",   olc::WHITE, this->fontSize);
    pge->DrawString(pos.x + offset.x + (80 + 8 * key1.size()) * fontSize, pos.y + offset.y , this->action,   olc::WHITE, this->fontSize);
}

void PressEntry::pressEntry2(olc::PixelGameEngine* pge, const olc::vi2d pos, olc::vi2d offset)
{
    pge->DrawString(pos.x + offset.x                 , pos.y + offset.y , "Press ", olc::WHITE, this->fontSize);
    pge->DrawString(pos.x + offset.x + 48  * fontSize, pos.y + offset.y , this->key1,     olc::GREEN, this->fontSize);
    pge->DrawString(pos.x + offset.x + (48 + 8 * key1.size())  * fontSize, pos.y + offset.y , ",",      olc::WHITE, this->fontSize);
    pge->DrawString(pos.x + offset.x + (56 + 8 * key1.size())  * fontSize, pos.y + offset.y , this->key2,      olc::GREEN, this->fontSize);
    pge->DrawString(pos.x + offset.x + (56 + 8 * key1.size() + 8 * key2.size())  * fontSize, pos.y + offset.y , " to ",   olc::WHITE, this->fontSize);
    pge->DrawString(pos.x + offset.x + (88 + 8 * key1.size() + 8 * key2.size()) * fontSize, pos.y + offset.y , this->action,   olc::WHITE, this->fontSize);
}

void PressEntry::drawYourself(olc::PixelGameEngine* pge, const olc::vi2d offset)
{
    if(key2.size() == 0)
    {
        pressEntry1(pge, {this->posX, this->posY}, offset);
    }
    else
    {
        pressEntry2(pge, {this->posX, this->posY}, offset);
    }
}

void Text::drawYourself(olc::PixelGameEngine* pge, const olc::vi2d offset)
{
    pge->DrawString(this->posX + offset.x, this->posY + offset.y, this->str, olc::WHITE, fontSize);
}

// cppcheck-suppress unusedFunction
void Menu::moveMenu(const int deltaX, const int deltaY)
{
    this->posX += deltaX;
    this->posY += deltaY;
}
// cppcheck-suppress unusedFunction
void Menu::resetPos()
{
    this->posX = 0;
    this->posY = 0;
}

void Menu::addContentToMenu(std::shared_ptr<MenuContent> content)
{
    contents.push_back(content);
}

void Menu::drawYourself(olc::PixelGameEngine* pge)
{
    pge->FillRect(posX, posY, windowSizeX, windowSizeY, olc::BLACK);
    pge->DrawRect(posX, posY, windowSizeX, windowSizeY, olc::WHITE);

    for(auto content : contents)
    {
        content->drawYourself(pge, {this->posX, this->posY});
    }
}

Menu::Menu()
{
    this->posX = 0;
    this->posY = 0;
    this->windowSizeX = 100;
    this->windowSizeY = 100;
}

Menu::Menu(int posX, int posY, unsigned short windowSizeX, unsigned short windowSizeY)
{
    this->posX = posX;
    this->posY = posY;
    this->windowSizeX = windowSizeX;
    this->windowSizeY = windowSizeY;
}

Menu::~Menu()
{
}
