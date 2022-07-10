#include "Box.h"

Box::Box(int x, int y, int width, int height, uint32_t color) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->color = color;
}

void Box::render(SDL_Renderer *renderer) {
  boxColor(renderer, x, y, x + width, y + height, color);
}