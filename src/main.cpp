#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "System.h"

int main(int argc, char **argv) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool isRunning = true;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Unable to init SDL : %s", SDL_GetError());
    return 1;
  }
  window = SDL_CreateWindow(
      "Nodesystem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    printf("Unable to create window : %s", SDL_GetError());
    return 1;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("Unable to create renderer : %s", SDL_GetError());
    return 1;
  }

  System system = System(renderer);

  // create render loop
  while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        isRunning = false;
      }
      system.handleEvent(&event);
    }
    system.render();
  }
  
  // Free
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return 0;
}