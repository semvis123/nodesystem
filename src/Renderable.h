#pragma once

class Renderable
{
public:
    virtual void render(SDL_Renderer *renderer) = 0;
};
