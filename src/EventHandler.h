#pragma once

#include <SDL2/SDL.h>

class EventHandler {
 public:
  virtual void handleEvent(SDL_Event *event) = 0;
};
