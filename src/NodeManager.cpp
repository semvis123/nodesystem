#include "NodeManager.h"

NodeManager::NodeManager() {
}

void NodeManager::registerForUpdate(Node *node) {
  printf("registerForUpdate node %d\n", node->id);
  nodesToUpdateLater.push({node, 0});
}

void NodeManager::registerForDelayedUpdate(Node *node, int delayTicks) {
  nodesToUpdateLater.push({node, delayTicks});
}

void NodeManager::update() {
  if (SDL_GetTicks() - lastTick > tickSpeed) {
    lastTick = SDL_GetTicks();
    performTick();
  }
}

void NodeManager::performTick() {
  for (int i = 0; i < nodesToUpdate.size(); i++) {
    auto &node = nodesToUpdate[i];
    if (node.delay == 0) {
      printf("updating node %d, delay %d\n", node.node->id, node.delay);
      node.node->update(this);
      // remove from list
      nodesToUpdate.erase(nodesToUpdate.begin() + i);
      i--;
    } else {
      node.delay--;
    }
  }

  while (!nodesToUpdateLater.empty()) {
    NodeWithDelay node = nodesToUpdateLater.top();
    nodesToUpdateLater.pop();
    nodesToUpdate.push_back(node);
  }
}