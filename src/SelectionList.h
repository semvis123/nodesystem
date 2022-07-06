#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Renderable.h"
#include "EventHandler.h"

class SelectionList: public Renderable, public EventHandler
{
private:
    std::vector<std::string> items;
    int x, y, width, height, selectedIndex = 0, scrollOffset = 0, itemHeight = 30, scrollWheelPosition = 0;
    bool hasSelection = false, isHovering = false, isPressed = false;
public:
    SelectionList(std::vector<std::string> items, int x, int y, int width, int height);
    void render(SDL_Renderer *renderer);
    void handleEvent(SDL_Event *event);
    bool inside(int x, int y);
    std::string getSelectedItem();
    void setSelectedItem(std::string item);
    void addItem(std::string item);
    bool removeItem(std::string item);
    void clearItems();
    void setSelectedIndex(int index);
};