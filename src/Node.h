#include <SDL2/SDL.h>
#include "Renderable.h"
#include "EventHandler.h"

class Node: public EventHandler, public Renderable
{
private:
    /* data */
public:
    Node(/* args */);
    void update();
    virtual void handleEvent(SDL_Event *event);
};
