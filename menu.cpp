#include "menu.hpp"

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

Text::Text(int posX, int posY, std::string str, uint32_t fontSize) : MenuContent(posX, posY) 
{
    this->str = str;
    this->fontSize = fontSize;
}

Text::~Text()
{
}

PressEntry::PressEntry(int posX, int posY, std::string key, std::string action, uint32_t fontSize) : MenuContent(posX, posY)
{
    this->posX = posX;
    this->posY = posY;

    this->key1 = key;
    this->action = action;
    this->fontSize = fontSize;
}

PressEntry::PressEntry(int posX, int posY, std::string key1, std::string key2, std::string action, uint32_t fontSize) : MenuContent(posX, posY)
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

void PressEntry::pressEntry(olc::PixelGameEngine* pge, const olc::vi2d pos, const olc::vi2d menuOffset, const std::string& key, const std::string& action, const uint32_t fontSize)
{
    pge->DrawString(pos.x + menuOffset.x                , pos.y + menuOffset.y , "Press ", olc::WHITE, fontSize);
    pge->DrawString(pos.x + menuOffset.x + 48 * fontSize, pos.y + menuOffset.y , key,      olc::GREEN, fontSize);
    pge->DrawString(pos.x + menuOffset.x + (48 + 8 * key.size()) * fontSize, pos.y + menuOffset.y , " to ",   olc::WHITE, fontSize);
    pge->DrawString(pos.x + menuOffset.x + (80 + 8 * key.size()) * fontSize, pos.y + menuOffset.y , action,   olc::WHITE, fontSize);
}

void PressEntry::pressEntry(olc::PixelGameEngine* pge, const olc::vi2d pos, const olc::vi2d menuOffset, const std::string& key1, const std::string& key2, const std::string& action, const uint32_t fontSize)
{
    pge->DrawString(pos.x + menuOffset.x                 , pos.y + menuOffset.y , "Press ", olc::WHITE, fontSize);
    pge->DrawString(pos.x + menuOffset.x + 48  * fontSize, pos.y + menuOffset.y , key1,     olc::GREEN, fontSize);
    pge->DrawString(pos.x + menuOffset.x + (48 + 8 * key1.size())  * fontSize, pos.y + menuOffset.y , ",",      olc::WHITE, fontSize);
    pge->DrawString(pos.x + menuOffset.x + (56 + 8 * key1.size())  * fontSize, pos.y + menuOffset.y , key2,      olc::GREEN, fontSize);
    pge->DrawString(pos.x + menuOffset.x + (56 + 8 * key1.size() + 8 * key2.size())  * fontSize, pos.y + menuOffset.y , " to ",   olc::WHITE, fontSize);
    pge->DrawString(pos.x + menuOffset.x + (88 + 8 * key1.size() + 8 * key2.size()) * fontSize, pos.y + menuOffset.y , action,   olc::WHITE, fontSize);
}

void PressEntry::drawYourself(olc::PixelGameEngine* pge, const olc::vi2d menuOffset)
{
    if(key2.size() == 0)
    {
        pressEntry(pge, {this->posX, this->posY}, menuOffset, key1, action, fontSize);
    }
    else
    {
        pressEntry(pge, {this->posX, this->posY}, menuOffset, key1, key2, action, fontSize);
    }
}

void Text::drawYourself(olc::PixelGameEngine* pge, const olc::vi2d menuOffset)
{
    pge->DrawString(this->posX + menuOffset.x, this->posY + menuOffset.y, this->str, olc::WHITE, fontSize);
}

void Menu::moveMenu(const int deltaX, const int deltaY)
{
    this->posX += deltaX;
    this->posY += deltaY;
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
