#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

class Button
{
private:
    /* data */
    int x, y, width, height;
    std::string label;
public:
    /* label, x, y, width, height */
    Button(std::string label, int x, int y, int width, int height);
    ~Button();
    void render(SDL_Renderer *renderer);
    void click(int x, int y);
};