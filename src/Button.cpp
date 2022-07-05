#include "Button.h"

Button::Button(std::string label, int x, int y, int width, int height)
{
    this->label = label;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

Button::~Button()
{
}

void Button::render(SDL_Renderer *renderer) {
    // create a rectangle
    SDL_Rect rect = {x, y, width, height};
    // draw the rectangle
    // check for hovering
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    uint32_t color = 0xFF444400;
    if (mouseX > x && mouseX < x + width && mouseY > y && mouseY < y + height) {
        color = 0xFF446600;
    }
    boxColor(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, color);
    
    // draw the label
    stringColor(renderer, x + width / 2 - label.length() * 4, y + height / 2 - 2, label.c_str(), 0xFFFFFFFF);
}

void Button::click(int x, int y) {
    if (x > this->x && x < this->x + this->width && y > this->y && y < this->y + this->height) {
        printf("Button clicked\n");
    }
}