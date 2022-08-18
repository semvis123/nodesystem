#include <queue>

#include "Node.h"

struct NodeWithDelay {
  Node *node;
  int delay;
};

auto cmp = [](NodeWithDelay a, NodeWithDelay b) { return a.delay - b.delay; };

class NodeManager {
 private:
  std::priority_queue<NodeWithDelay, std::vector<NodeWithDelay>, decltype(cmp)> nodesToUpdate;
  bool isRunning = false;
  int tickSpeed = 60;  // 1 tick per frame, 60 ticks per second, should change this in the future
  int tickCountDown = 0;

 public:
  void start();
  void stop();
  void update();
  void registerForUpdate(Node *node);
  void registerForDelayedUpdate(Node *node, int delayTicks);
};
