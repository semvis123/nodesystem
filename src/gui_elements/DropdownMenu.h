#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <string>
#include <vector>

#include "Button.h"
#include "DropdownOption.h"

class DropdownMenu : public Renderable, public EventHandler {
 private:
  int x, y, width, height;
  std::string label;
  bool isOpen = false, isHovering = false, isPressed = false;
  std::vector<Button *> options;

 public:
  DropdownMenu(std::string label, int x, int y, int width, int height, std::vector<DropdownOption> options);
  virtual void render(SDL_Renderer *renderer);
  virtual void handleEvent(SDL_Event *event);
  bool inside(int x, int y);
};