#pragma once

#include <SDL2/SDL.h>

#include "./NodeManager.h"
#include "EventHandler.h"
#include "Object.h"
#include "Renderable.h"

typedef class NodeManager NodeManager;

struct NodeConnection {
  std::vector<Node *> nodes;
  std::string name;
}

class DisplayNode : public Node {
 private:
  /* data */
  int value = false;
  std::vector<NodeConnection> inputs = nullptr;
  std::vector<NodeConnection> outputs;

 public:
  Node(int x, int y, int width, int height, int id);
  void update(NodeManager *nodeManager);
  virtual void render(SDL_Renderer *renderer);
  virtual void handleEvent(SDL_Event *event);
  void setInput(Node *input);
  void addOutput(Node *output);
  void removeOutput(Node *output);
  int getValue();
  int id;
};
