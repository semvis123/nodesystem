#include "SelectionList.h"

SelectionList::SelectionList(std::vector<std::string> items, int x, int y, int width, int height) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->items = items;
}

void SelectionList::render(SDL_Renderer *renderer) {
  uint32_t bgColor = 0xFF336622;
  uint32_t outlineColor = 0xFF000000;
  uint32_t selectedColor = 0xFF229900;
  uint32_t indicatorColor = 0xFF154522;
  uint32_t textColor = 0xFFFFFFFF;
  uint32_t hoverColor = 0xFF225500;

  int fontHeight = 6;

  int w = width - scrollBarWidth;
  int h = height;

  boxColor(renderer, x, y, x + width, y + height, bgColor);

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
        boxColor(renderer, x + w / 4, y + h, x + w / 4 * 3, y + h - 4, indicatorColor);
      }
    } else {
      int color = bgColor;
      if (i == selectedIndex) {
        color = selectedColor;
      } else if (i == hoveringIndex) {
        color = hoverColor;
      }
      boxColor(renderer, x, itemPositionY, x + w - 1, itemPositionY + itemHeight - 1, color);
      rectangleColor(renderer, x, itemPositionY, x + w, itemPositionY + itemHeight, outlineColor);
      stringColor(renderer, textPositionX, textPositionY, items[i].c_str(), textColor);
    }
  }

  // draw the scroll bar
  int scrollBarHeight = getScrollBarHeight();
  int maxScrollable = (items.size() - (h / itemHeight));
  int scrollBarOffset = (h - scrollBarHeight) * scrollOffset / maxScrollable;
  int sX = x + w;
  int sY = y + scrollBarOffset;
  boxColor(renderer, sX, sY, sX + scrollBarWidth, sY + scrollBarHeight, indicatorColor);
  rectangleColor(renderer, sX, sY, sX + scrollBarWidth, sY + scrollBarHeight, outlineColor);
}

void SelectionList::handleEvent(SDL_Event *event) {
  int x, y;
  SDL_GetMouseState(&x, &y);
  bool isInside = inside(x, y);
  bool isInsideItem = x < this->x + (width - scrollBarWidth);
  int itemIndex = findItemIndexAt(x, y);

  // scroll bar
  int maxScrollable = (items.size() - (height / itemHeight));
  int scrollBarOffset = (height - getScrollBarHeight()) * scrollOffset / maxScrollable;
  int scrollBarHeight = getScrollBarHeight();

  switch (event->type) {
    case SDL_MOUSEBUTTONDOWN:
      if (isInside) {
        if (isInsideItem) {
          if (itemIndex == selectedIndex) {
            selectedIndex = -1;
          } else {
            selectedIndex = itemIndex;
          }
        } else {
          // scroll bar
          if (y < this->y + scrollBarOffset) {
            scrollOffset -= 1;
          } else if (y > this->y + scrollBarOffset + getScrollBarHeight()) {
            scrollOffset += 1;
          } else {
            // scroll bar grabbed
            hasScrollBar = true;
            // check at what height the scroll bar is grabbed
            scrollBarGrabY = y - this->y - scrollBarOffset;
          }
        }
      }
      break;
    case SDL_MOUSEMOTION:
      if (isInside && isInsideItem) {
        hoveringIndex = itemIndex;
      } else if (hasScrollBar && (height - scrollBarHeight) > 0) {
        // calculate the new scroll offset
        scrollOffset = (y - this->y - scrollBarGrabY) * maxScrollable / (height - scrollBarHeight);
        scrollOffset = fmax(0, fmin(maxScrollable, scrollOffset));
      } else {
        hoveringIndex = -1;
      }
      break;
    case SDL_MOUSEBUTTONUP:
      hasScrollBar = false;
      break;
    case SDL_MOUSEWHEEL:
      if (isInside) {
        int scrollInterval = 10;
        // check if we moved the scrollbar by mouse
        if (fmin(fmax(scrollWheelPosition / scrollInterval, 0), maxScrollable) != scrollOffset) {
          scrollWheelPosition = scrollOffset * scrollInterval;
        }

        scrollWheelPosition -= abs(event->wheel.y) > 3 ? event->wheel.y : 10 * event->wheel.y;
        scrollWheelPosition = fmax(scrollWheelPosition, 0);
        scrollWheelPosition = fmin(scrollWheelPosition, maxScrollable * scrollInterval);
        scrollOffset = fmax(scrollWheelPosition / scrollInterval, 0);
        scrollOffset = fmin(scrollOffset, maxScrollable);
      }
      break;
    default:
      break;
  }
}

int SelectionList::findItemIndexAt(int x, int y) {
  int maxScrollable = (items.size() - (height / itemHeight));
  int topSpace = 0;
  if (scrollOffset > 0 && (items.size() - scrollOffset) * itemHeight < height) {
    topSpace = -((items.size() - scrollOffset) * itemHeight - height);
  }

  if (y < this->y + topSpace) {
    return -1;
  }

  int itemIndex = (y - this->y - topSpace) / itemHeight + scrollOffset;
  if (itemIndex >= items.size()) {
    return -1;
  }
  return itemIndex;
}

bool SelectionList::inside(int x, int y) {
  return x >= this->x && x <= this->x + this->width && y >= this->y && y <= this->y + this->height;
}

std::string SelectionList::getSelectedItem() {
  return items[selectedIndex];
}

void SelectionList::setSelectedItem(std::string item) {
  for (int i = 0; i < items.size(); i++) {
    if (items[i] == item) {
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
  for (int i = 0; i < items.size(); i++) {
    if (items.at(i) == item) {
      items.erase(items.begin() + i);
      found = true;
      if (selectedIndex >= i) {
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

int SelectionList::getScrollBarHeight() {
  return fmax(((80 * height) / (items.size() * itemHeight)), 40);
}