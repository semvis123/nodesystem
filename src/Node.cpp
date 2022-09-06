#include "Node.h"

Node::Node(int x, int y, int width, int height, int id) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->id = id;
  std::vector<Node *> nodeList = std::vector<Node *>();
  std::string outputName = "1";
  NodeConnectionOutput output = {nodeList, outputName, false};
  std::vector<Node *> nodeListInputs = std::vector<Node *>();
  NodeConnection input = {nodeListInputs, outputName, 1};
  this->outputs.push_back(output);
  this->inputs.push_back(input);
}

void Node::handleEvent(SDL_Event *event) {
}

void Node::update(NodeManager *nodeManager) {
  printf("update node %d\n", id);
  if (inputs.empty() || inputs[0].nodes.empty()) {
    outputs[0].value = !outputs[0].value;
  } else {
    if (!inputs[0].nodes.empty()) {
      outputs[0].value = !inputs[0].nodes[0]->outputs[0].value;
    }
  }
  printf("node %d value is %d\n", id, outputs[0].value);
  printf("node %d outputs size is %d\n", id, outputs.size());
  if (!outputs.empty()) {
    for (int i = 0; i < outputs[0].nodes.size(); i++) {
      nodeManager->registerForUpdate(outputs[0].nodes[i]);
    }
  }

  if (id == 0) nodeManager->registerForDelayedUpdate(this, 60);
  printf("node %d updated\n", id);
}

void Node::render(SDL_Renderer *renderer) {
  SDL_Rect rect = {x, y, width, height};
  SDL_SetRenderDrawColor(renderer, outputs[0].value ? 255 : 0, outputs[0].value ? 0 : 255, 0, 255);
  SDL_RenderFillRect(renderer, &rect);
}

void Node::setInput(Node *input) {
  if (this->inputs.empty()) return;
  if (!this->inputs[0].nodes.empty()) {
    this->inputs[0].nodes[0]->removeOutput(this);
  }
  this->inputs[0].nodes.push_back(input);
}

void Node::addOutput(Node *output) {
  if (this->outputs.empty()) return;
  outputs[0].nodes.push_back(output);
  output->setInput(this);
}

void Node::removeOutput(Node *output) {
  if (this->outputs.empty()) return;
  for (int i = 0; i < outputs[0].nodes.size(); i++) {
    if (outputs[0].nodes[i] == output) {
      outputs[0].nodes.erase(outputs[0].nodes.begin() + i);
      output->setInput(nullptr);
      return;
    }
  }
}