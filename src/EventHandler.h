#pragma once

class EventHandler
{
public:
    virtual void handleEvent(SDL_Event *event) = 0;
};

