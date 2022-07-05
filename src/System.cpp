#include "System.h"


System::System(SDL_Renderer *renderer)
{
    this->renderer = renderer;
    this->button = new Button("Hello", 0, 0, 80, 30);
}

System::~System()
{
}


void System::render()
{
    SDL_Renderer *renderer = this->renderer;
    // Clear screen
    SDL_RenderClear(renderer);
    // display all the nodes in the system
    for (int i = 0; i < 10; i++)
    {
        int x = i * 50;
        int y = i * 50;
        filledCircleRGBA(renderer, x, y, 20, 0, 255, 0, 255);
    }

    // display all the buttons in the system
    button->render(renderer);

    SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);
    SDL_Delay(1000 / 60);
    // Render modification
    SDL_RenderPresent(renderer);
}

void System::click(int x, int y)
{
    button->click(x, y);
}