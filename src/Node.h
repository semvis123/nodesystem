#pragma once

#include <SDL2/SDL.h>

#include <any>
#include <string>

#include "./NodeManager.h"
#include "EventHandler.h"
#include "Node.h"
#include "Object.h"
#include "Renderable.h"

typedef class Node Node;
typedef class NodeManager NodeManager;
struct NodeConnection {
  std::vector<Node *> nodes;
  std::string name;
  int id;
};

struct NodeConnectionOutput {
  std::vector<Node *> nodes;
  std::string name;
  bool value;
};

class Node : public EventHandler, public Renderable, public Object {
 private:
  /* data */
  std::vector<NodeConnection> inputs;
  std::vector<NodeConnectionOutput> outputs;

 public:
  Node(int x, int y, int width, int height, int id);
  void update(NodeManager *nodeManager);
  virtual void render(SDL_Renderer *renderer);
  virtual void handleEvent(SDL_Event *event);
  void setInput(Node *input);
  void addOutput(Node *output);
  void removeOutput(Node *output);
  int id;
};
