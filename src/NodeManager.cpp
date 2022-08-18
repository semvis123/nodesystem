#include "NodeManager.h"

void NodeManager::registerForUpdate(Node *node) {
  nodesToUpdate.push({node, 1});
}

void NodeManager::registerForDelayedUpdate(Node *node, int delayTicks) {
  nodesToUpdate.push({node, delayTicks});
}