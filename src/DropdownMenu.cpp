#include "DropdownMenu.h"

DropdownMenu::DropdownMenu(std::string label, int x, int y, int width, int height, std::vector<DropdownOption> options) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->label = label;
    for (int i = 0; i < options.size(); i++) {
        Button *button = new Button(options[i].name, x, y + (i + 1) * height, width, height, options[i].value);
        this->options.push_back(button);
    }
}

void DropdownMenu::render(SDL_Renderer *renderer) {
    SDL_Rect rect = {x, y, width, height};
    uint32_t color = 0xFF223311;
    if (isPressed) {
        color = 0xFF229900;
    } else if (isHovering) {
        color = 0xFF444400;
    }
    boxColor(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, color);
    stringColor(renderer, x + width / 2 - label.length() * 4, y + height / 2 - 2, label.c_str(), 0xFFFFFFFF);
    if (isOpen) {
        for (int i = 0; i < options.size(); i++) {
            options[i]->render(renderer);
        }
    }
}

void DropdownMenu::handleEvent(SDL_Event *event) {
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
                isOpen = !isOpen;
            }
            isPressed = false;
            break;
        case SDL_MOUSEMOTION:
            isHovering = isInside;
            break;
        default:
            break;
    }

    if (isOpen) {
        for (int i = 0; i < options.size(); i++) {
            options[i]->handleEvent(event);
        }
    }
}

bool DropdownMenu::inside(int xPos, int yPos) {
    return xPos > x && xPos < x + width && yPos > y && yPos < y + height;
}