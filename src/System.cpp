#include "System.h"


System::System(SDL_Renderer *renderer)
{
    this->renderer = renderer;
    for (int i = 0; i < 3; i++)
    {
        Button *button = new Button("Button " + std::to_string(i + 1), 100 + i * 100, 0, 100, 50, [i]() {
            printf("Button clicked %d\n", i);
        });
        eventHandlers.push_back(button);
        renderables.push_back(button);
    }


    std::vector<DropdownOption> options;
    options.push_back({"option1", []() {
        printf("option1\n");
    }});
    options.push_back({"option2", []() {
        printf("option2\n");
    }});

    DropdownMenu *dropdownMenu = new DropdownMenu("Dropdown", 0, 0, 100, 50, options);
    eventHandlers.push_back(dropdownMenu);
    renderables.push_back(dropdownMenu);
}

System::~System()
{
}


void System::render()
{
    SDL_RenderClear(renderer);

    for (int i = 0; i < renderables.size(); i++)
    {
        renderables[i]->render(renderer);
    }

    SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255); // background color
    SDL_Delay(1000 / 60);
    SDL_RenderPresent(renderer);
}

void System::handleEvent(SDL_Event *event)
{
    for (int i = 0; i < eventHandlers.size(); i++)
    {
        eventHandlers[i]->handleEvent(event);
    }
}