#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Button.h"

class System
{
private:
    SDL_Renderer *renderer;
    Button *button;
public:
    System(SDL_Renderer *renderer);
    ~System();
    void render();
    void click(int x, int y);
};