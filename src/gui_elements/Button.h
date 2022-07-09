#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <functional>
#include <string>

#include "EventHandler.h"
#include "Renderable.h"

class Button : public EventHandler, public Renderable {
 private:
  int x, y, width, height;
  std::string label;
  std::function<void()> callback;
  bool isHovering = false, isPressed = false;
  bool inside(int x, int y);

 public:
  Button(std::string label, int x, int y, int width, int height, std::function<void()> callback);
  virtual void render(SDL_Renderer *renderer);
  virtual void handleEvent(SDL_Event *event);
  void onClick();
  void onHover();
  void onLeave();
  void onMouseDown();
};