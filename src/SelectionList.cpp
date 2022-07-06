#include "SelectionList.h"

SelectionList::SelectionList(std::vector<std::string> items, int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->items = items;
}

void SelectionList::render(SDL_Renderer *renderer)
{
    uint32_t color = 0xFF336622;
    uint32_t outlineColor = 0xFF000000;
    uint32_t selectedColor = 0xFF229900;
    uint32_t indicatorColor = 0xFF154522;
    uint32_t textColor = 0xFFFFFFFF;

    int fontHeight = 6;
    int scrollBarWidth = 10;

    int w = width - scrollBarWidth;
    int h = height;

    boxColor(renderer, x, y, x + width, y + height, color);

    int topSpace = 0;
    if (scrollOffset > 0 && (items.size() - scrollOffset) * itemHeight < height) {
        topSpace = -((items.size() - scrollOffset) * itemHeight - h);
        // draw a indicator to indicate that there are items above
        lineColor(renderer, x, y, x, y + topSpace, outlineColor);
        lineColor(renderer, x + w - 1, y, x + w - 1, y + topSpace, outlineColor);
        lineColor(renderer, x, y + topSpace - 1, x + w - 1, y + topSpace - 1, outlineColor);
        boxColor(renderer, x + w / 4, y, x + w / 4 * 3, y + 4, indicatorColor);
    }

    for (int i = scrollOffset; i < items.size() && (i - scrollOffset) * itemHeight < h; i++) {
        int itemPositionY = y + (i - scrollOffset) * itemHeight + topSpace;
        int textPositionX = x + w / 2 - items[i].length() * 4;
        int textPositionY = itemPositionY + itemHeight / 2 - 2;

        // item that can only be partially drawn
        if ((i - scrollOffset + 1) * itemHeight >= h) {
            // we only need to draw the upper lines
            lineColor(renderer, x, itemPositionY, x + w - 1, itemPositionY, outlineColor);
            lineColor(renderer, x, itemPositionY, x, y + h, outlineColor);
            lineColor(renderer, x + w - 1, itemPositionY, x + w - 1, y + h, outlineColor);
            
            // check if the text is visible
            if (-((i - scrollOffset) * itemHeight - h) > itemHeight / 2 + fontHeight / 2) {
                stringColor(renderer, textPositionX, textPositionY, items[i].c_str(), textColor);
            } else {
                // show that there is another item
                boxColor(renderer, x + w / 4, textPositionY, x + w / 4 * 3, textPositionY + 4, indicatorColor);
            }
        } else {
            rectangleColor(renderer, x, itemPositionY, x + w, itemPositionY + itemHeight, outlineColor);
            stringColor(renderer, textPositionX, textPositionY, items[i].c_str(), textColor);
        }
    }

    // draw the scroll bar
    int scrollBarHeight = h * h / (items.size() * itemHeight);
    // scrollOffset is the amount of items that are hidden at the top
    int maxScrollable = (items.size() - (h / itemHeight));
    int scrollBarOffset = (h - scrollBarHeight) * scrollOffset / maxScrollable;
    boxColor(renderer, x + w, y + scrollBarOffset, x + w + scrollBarWidth, y + scrollBarOffset + scrollBarHeight, indicatorColor);
    rectangleColor(renderer, x + w, y + scrollBarOffset, x + w + scrollBarWidth, y + scrollBarOffset + scrollBarHeight, outlineColor);
}
void SelectionList::handleEvent(SDL_Event *event)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    bool isInside = inside(x, y);
    
    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN:
            if (isInside) {
                isPressed = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (isInside && isPressed) {
                // isOpen = !isOpen;
            }
            isPressed = false;
            break;
        case SDL_MOUSEMOTION:
            isHovering = isInside;
            break;
        case SDL_MOUSEWHEEL:
            if (isInside) {
                int scrollInterval = 10;
                scrollWheelPosition -= event->wheel.y;
                scrollWheelPosition = fmax(scrollWheelPosition, 0);
                scrollWheelPosition = fmin(scrollWheelPosition, (items.size() - (
                    height / itemHeight)) * scrollInterval);
                scrollOffset = fmax(scrollWheelPosition / scrollInterval, 0);
            }
            break;
        default:
            break;
    }
}

bool SelectionList::inside(int x, int y) {
    return x >= this->x && x <= this->x + this->width && y >= this->y && y <= this->y + this->height;
}

std::string SelectionList::getSelectedItem() {
    return items[selectedIndex];
}

void SelectionList::setSelectedItem(std::string item) {
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i] == item)
        {
            selectedIndex = i;
            break;
        }
    }
}

void SelectionList::addItem(std::string item) {
    items.push_back(item);
}

bool SelectionList::removeItem(std::string item) {
    bool found = false;
    for (int i = 0; i < items.size(); i++)
    {
        if (items.at(i) == item)
        {
            items.erase(items.begin() + i);
            found = true;
            if (selectedIndex >= i)
            {
                selectedIndex--;
            }
            break;
        }
    }
    return found;
}
void SelectionList::clearItems() {
    items.clear();
}

void SelectionList::setSelectedIndex(int index) {
    selectedIndex = index;
}