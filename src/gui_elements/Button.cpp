#include "Button.h"

Button::Button(std::string label, int x, int y, int width, int height, std::function<void()> callback) {
  this->label = label;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->callback = callback;
}

void Button::render(SDL_Renderer *renderer) {
  SDL_Rect rect = {x, y, width, height};
  uint32_t color = isHovering ? 0xFF444400 : 0xFF446600;
  if (isPressed) {
    color = 0xFF229900;
  }
  boxColor(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, color);
  stringColor(renderer, x + width / 2 - label.length() * 4, y + height / 2 - 2, label.c_str(), 0xFFFFFFFF);
}

void Button::handleEvent(SDL_Event *event) {
  int x, y;
  SDL_GetMouseState(&x, &y);
  bool isInside = inside(x, y);
  switch (event->type) {
    case SDL_MOUSEBUTTONDOWN:
      if (isInside) {
        isPressed = true;
        onMouseDown();
      }
      break;
    case SDL_MOUSEBUTTONUP:
      if (isInside && isPressed) {
        onClick();
      }
      isPressed = false;
      break;
    case SDL_MOUSEMOTION:
      if (isHovering != isInside) {
        if (isInside) {
          onHover();
        } else {
          onLeave();
        }
      }
      isHovering = isInside;
      break;
    default:
      break;
  }
}

void Button::onClick() {
  callback();
}

void Button::onMouseDown() {
  // printf("Button mouse down\n");
}

void Button::onHover() {
  // printf("Button hover\n");
}

void Button::onLeave() {
  // printf("Button leave\n");
}

bool Button::inside(int xPos, int yPos) {
  return xPos > x && xPos < x + width && yPos > y && yPos < y + height;
}