#include "Text.h"

Text::Text(std::string text, int x, int y, uint32_t color) {
  this->text = text;
  this->x = x;
  this->y = y;
  this->color = color;
}

void Text::render(SDL_Renderer *renderer) {
  stringColor(renderer, x, y, text.c_str(), color);
}