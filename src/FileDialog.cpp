#include "FileDialog.h"

FileDialog::FileDialog(FileDialogMode mode, std::string title, std::string initialPath,
                       std::vector<std::string> extensions, int x, int y, int width, int height) {
  this->mode = mode;
  this->title = title;
  this->filePath = initialPath;
  this->initialPath = initialPath;
  this->extensions = extensions;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;

  this->renderables = {};
  this->eventHandlers = {};
  this->callback = nullptr;
}

void FileDialog::showFileDialog() {
  if (!renderables.size()) createElements();

  visible = true;
}

void FileDialog::render(SDL_Renderer *renderer) {
  if (!visible) {
    return;
  }

  for (auto renderable : renderables) {
    renderable->render(renderer);
  }
}

void FileDialog::handleEvent(SDL_Event *event) {
  if (!visible) {
    return;
  }
  for (auto eventHandler : eventHandlers) {
    eventHandler->handleEvent(event);
  }
}

std::vector<std::string> getFiles(std::string path, std::vector<std::string> extensions) {
  // Get all filenames in the given path with the given extensions if any
  std::vector<std::string> files;
  for (auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      std::string filename = entry.path().filename().string();
      if (extensions.size() == 0) {
        files.push_back(filename);
      } else {
        for (auto extension : extensions) {
          if (filename.find(extension) != std::string::npos) {
            files.push_back(filename);
          }
        }
      }
    } else if (entry.is_directory()) {
      files.push_back("* " + entry.path().filename().string());
    }
  }
  printf("%d files found\n", files.size());
  return files;
}

void FileDialog::createElements() {
  int buttonPadding = 10;
  int buttonWidth = (width - (buttonPadding * 3)) / 2;
  int buttonHeight = 30;
  int buttonX = x + buttonPadding;
  int buttonY = y + height - buttonHeight - buttonPadding;

  renderables.clear();
  eventHandlers.clear();

  // background
  Box *background = new Box(x, y, width, height, 0xFF000000);
  renderables.push_back(background);

  // Create the file dialog title.
  Text *titleText = new Text(title, x + 10, y + 10, 0xFFFFFFFF);
  renderables.push_back(titleText);

  // Create the file dialog list.
  std::vector<std::string> files = getFiles(initialPath, extensions);
  int listWidth = width - (buttonPadding * 2);
  int listHeight = height - (buttonPadding * 3) - buttonHeight;
  int listX = x + buttonPadding;
  int listY = y + buttonPadding;
  SelectionList *fileList = new SelectionList(files, listX, listY, listWidth, listHeight);
  renderables.push_back(fileList);
  eventHandlers.push_back(fileList);

  // Create the file dialog buttons.
  Button *openButton =
      new Button("Cancel", buttonX, buttonY, buttonWidth, buttonHeight, [this]() { this->callback({}); });
  renderables.push_back(openButton);
  eventHandlers.push_back(openButton);

  Button *saveButton = new Button("Open", buttonX + buttonWidth + buttonPadding, buttonY, buttonWidth,
                                  buttonHeight, [this, fileList]() {
                                    filePath = fileList->getSelectedItem();
                                    this->callback(this->filePath);
                                  });
  renderables.push_back(saveButton);
  eventHandlers.push_back(saveButton);
}

std::string FileDialog::getFilePath() {
  return filePath;
}

void FileDialog::setCallback(std::function<void(std::optional<std::string>)> callback) {
  this->callback = callback;
}

void FileDialog::close() {
  visible = false;
}