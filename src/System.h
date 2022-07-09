#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <functional>
#include <string>
#include <vector>

#include "gui_elements/gui_elements.h"

class System {
 private:
  SDL_Renderer *renderer;
  std::vector<EventHandler *> eventHandlers;
  std::vector<Renderable *> renderables;

 public:
  System(SDL_Renderer *renderer);
  ~System();
  void render();
  void handleEvent(SDL_Event *event);
  void createMenu();
};