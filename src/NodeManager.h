#pragma once

#include <queue>
#include <stack>

#include "Node.h"

typedef class Node Node;
struct NodeWithDelay {
  Node *node;
  int delay;
};

class NodeManager {
 private:
  std::deque<NodeWithDelay> nodesToUpdate;
  std::stack<NodeWithDelay> nodesToUpdateLater;
  bool isRunning = false;
  int tickSpeed = 1000 / 60;  // 1 tick per frame, 60 ticks per second, should change this in the future
  int lastTick = 0;

 public:
  NodeManager();
  void update();
  void performTick();
  void registerForUpdate(Node *node);
  void registerForDelayedUpdate(Node *node, int delayTicks);
};
