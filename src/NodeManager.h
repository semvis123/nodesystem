#include <queue>

#include "Node.h"

struct NodeWithDelay {
  Node *node;
  int delay;
};

auto cmp = [](NodeWithDelay a, NodeWithDelay b) { return a.delay - b.delay; };

class NodeManager {
 private:
  std::vector<NodeWithDelay> nodesToUpdate;
  bool isRunning = false;
  int tickSpeed = 1000 / 1;  // 1 tick per frame, 60 ticks per second, should change this in the future
  int lastTick = 0;

 public:
  void update();
  void performTick();
  void registerForUpdate(Node *node);
  void registerForDelayedUpdate(Node *node, int delayTicks);
};
