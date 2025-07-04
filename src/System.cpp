#include "System.h"

System::System(SDL_Renderer *renderer, SDL_Window *window) {
  this->renderer = renderer;
  this->window = window;
  this->nodeManager = new NodeManager();

  createMenu();
}

void System::render() {
  nodeManager->update();

  SDL_RenderClear(renderer);

  for (int i = 0; i < renderables.size(); i++) {
    renderables[i]->render(renderer);
  }

  SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);  // background color
  SDL_Delay(1000 / 60);
  SDL_RenderPresent(renderer);
}

void System::handleEvent(SDL_Event *event) {
  for (int i = 0; i < eventHandlers.size(); i++) {
    eventHandlers[i]->handleEvent(event);
  }

  // TODO: handle resize event, recenter the filedialog maybe?
  // position anchors would be nice too, but that's a bit more work.
}

void System::createMenu() {
  // get window width and height
  int windowWidth, windowHeight;
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);

  // center the file dialog
  int x = (windowWidth - 400) / 2;
  int y = (windowHeight - 400) / 2;

  // get the current executable path
  char *path = SDL_GetBasePath();
  FileDialog *fileDialog = new FileDialog(FileDialogMode::Open, "Open File", path, {}, x, y, 400, 400);
  SDL_free(path);
  fileDialog->setCallback([this, fileDialog](std::optional<std::string> filePath) {
    if (filePath.has_value()) {
      printf("File path: %s\n", filePath.value().c_str());
      SDL_SetWindowTitle(window, ("Nodesystem - " + filePath.value()).c_str());
    } else {
      printf("File dialog was canceled\n");
      fileDialog->close();
    }
  });

  Node *node = new Node(x + 100, y + 100, 100, 100, 0);
  renderables.push_back(node);
  eventHandlers.push_back(node);
  nodeManager->registerForUpdate(node);

  Node *node2 = new Node(x + 250, y + 100, 100, 100, 1);
  renderables.push_back(node2);
  eventHandlers.push_back(node2);

  node->addOutput(node2);

  eventHandlers.push_back(fileDialog);
  renderables.push_back(fileDialog);

  std::vector<DropdownOption> options;
  options.push_back({"New", []() -> bool {
                       printf("New\n");

                       return true;
                     }});

  options.push_back({"Open", [fileDialog]() -> bool {
                       fileDialog->showFileDialog();
                       return true;
                     }});
  options.push_back({"Save", []() {
                       printf("save file\n");
                       return false;
                     }});
  options.push_back({"Save as", []() {
                       printf("save as\n");
                       return false;
                     }});

  DropdownMenu *dropdownMenu = new DropdownMenu("File", 0, 0, 100, 30, options);
  eventHandlers.push_back(dropdownMenu);
  renderables.push_back(dropdownMenu);

  // SelectionList *selectionList = new SelectionList(
  //     {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Item 6", "Item 7", "Item 8", "Item 9", "Item
  //     10"}, 200, 100, 200, 200);
  // eventHandlers.push_back(selectionList);
  // renderables.push_back(selectionList);

  // EditableTextBox *editableTextBox = new EditableTextBox("Placeholder", "", 400, 100, 200, 30);
  // eventHandlers.push_back(editableTextBox);
  // renderables.push_back(editableTextBox);
}