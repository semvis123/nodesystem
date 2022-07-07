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
    int x, y, width, height;
    int selectedIndex = -1, hoveringIndex = -1, scrollOffset = 0, itemHeight = 30, scrollWheelPosition = 0;
    int scrollBarWidth = 10;
    bool hasScrollBar = false;
    int scrollBarGrabY = 0;
    bool inside(int x, int y);
    int findItemIndexAt(int x, int y);
    int getScrollBarHeight();
public:
    SelectionList(std::vector<std::string> items, int x, int y, int width, int height);
    void render(SDL_Renderer *renderer);
    void handleEvent(SDL_Event *event);
    std::string getSelectedItem();
    void setSelectedItem(std::string item);
    void addItem(std::string item);
    bool removeItem(std::string item);
    void clearItems();
};