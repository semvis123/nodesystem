#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <functional>
#include <string>
#include <vector>

#include "FileDialog.h"
#include "NodeManager.h"
#include "gui_elements/gui_elements.h"

class System {
 private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  std::vector<EventHandler *> eventHandlers;
  std::vector<Renderable *> renderables;
  NodeManager *nodeManager;

 public:
  System(SDL_Renderer *renderer, SDL_Window *window);
  void render();
  void handleEvent(SDL_Event *event);
  void createMenu();
};