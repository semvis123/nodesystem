#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <functional>
#include <string>

#include "../Positional.h"
#include "../Sizable.h"
#include "EventHandler.h"
#include "Renderable.h"

class Button : public EventHandler, public Renderable, virtual public Positional, virtual public Sizable {
 private:
  std::string label;
  std::function<void()> callback;
  bool isHovering = false, isPressed = false, isDisabled = false;
  bool inside(int x, int y);

 public:
  Button(std::string label, int x, int y, int width, int height, std::function<void()> callback);
  virtual void render(SDL_Renderer *renderer);
  virtual void handleEvent(SDL_Event *event);
  void onClick();
  void onHover();
  void onLeave();
  void onMouseDown();
  void setDisabled(bool disabled);
};