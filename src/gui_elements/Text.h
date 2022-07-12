#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <string>

#include "../Positional.h"
#include "../Renderable.h"

class Text : public Renderable, virtual Positional {
 private:
  std::string text;
  uint32_t color;

 public:
  Text(std::string text, int x, int y, uint32_t color);
  void render(SDL_Renderer *renderer);
};