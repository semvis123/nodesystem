#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "../Positional.h"
#include "../Renderable.h"
#include "../Sizable.h"

class Box : public Renderable, virtual public Positional, virtual public Sizable {
 private:
  uint32_t color;

 public:
  Box(int x, int y, int width, int height, uint32_t color);
  void render(SDL_Renderer *renderer);
};
