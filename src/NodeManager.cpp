#include "NodeManager.h"

void NodeManager::registerForUpdate(Node *node) {
  nodesToUpdate.push_back({node, 1});
}

void NodeManager::registerForDelayedUpdate(Node *node, int delayTicks) {
  nodesToUpdate.push_back({node, delayTicks});
}

void NodeManager::update() {
  if (SDL_GetTicks() - lastTick > tickSpeed) {
    lastTick = SDL_GetTicks();
    performTick();
  }
}

void NodeManager::performTick() {
  printf("performTick\n");
  for (int i = 0; i < nodesToUpdate.size(); i++) {
    auto &node = nodesToUpdate[i];
    if (node.delay == 0) {
      node.node->update();
      nodesToUpdate.erase(nodesToUpdate.begin() + i);
    } else {
      node.delay--;
    }
  }
}