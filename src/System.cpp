#include "System.h"

System::System(SDL_Renderer *renderer) {
  this->renderer = renderer;
  createMenu();
}

System::~System() {
}

void System::render() {
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
}

void System::createMenu() {
  std::vector<DropdownOption> options;
  options.push_back({"Open", []() { printf("open file\n"); }});
  options.push_back({"Save", []() { printf("save file\n"); }});
  options.push_back({"Save as", []() { printf("save as\n"); }});

  DropdownMenu *dropdownMenu = new DropdownMenu("File", 0, 0, 100, 30, options);
  eventHandlers.push_back(dropdownMenu);
  renderables.push_back(dropdownMenu);

  SelectionList *selectionList = new SelectionList(
      {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Item 6", "Item 7", "Item 8", "Item 9", "Item 10"}, 200, 100,
      200, 200);
  eventHandlers.push_back(selectionList);
  renderables.push_back(selectionList);

  EditableTextBox *editableTextBox = new EditableTextBox("Placeholder", "", 400, 100, 200, 30);
  eventHandlers.push_back(editableTextBox);
  renderables.push_back(editableTextBox);
}