#include "FileDialog.h"

std::string getParentDirectory(std::string path) {
  // Get the parent directory of the given path
  std::string parentDirectory = path;
  char directorySeparators[] = {'/', '\\'};
  for (auto &separator : directorySeparators) {
    bool found = false;
    if (parentDirectory.back() == separator) {
      parentDirectory.pop_back();
      found = true;
    }

    size_t lastSlash = parentDirectory.find_last_of(separator);
    size_t firstSlash = parentDirectory.find_first_of(separator);
    if (lastSlash != std::string::npos && lastSlash != firstSlash) {
      parentDirectory = parentDirectory.substr(0, lastSlash);
      found = true;
    }
    if (found) {
      break;
    }
  }

  return parentDirectory;
}

FileDialog::FileDialog(FileDialogMode mode, std::string title, std::string initialPath,
                       std::vector<std::string> extensions, int x, int y, int width, int height) {
  this->mode = mode;
  this->title = title;
  this->filePath = getParentDirectory(initialPath);
  this->initialPath = this->filePath;
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

std::vector<NamedItem> getFiles(std::string path, std::vector<std::string> extensions) {
  // Get all filenames in the given path with the given extensions if any
  std::vector<NamedItem> files;
  for (auto &entry : std::filesystem::directory_iterator(path)) {
    std::string filename = entry.path().filename().string();
    std::string extension = entry.path().extension().string();
    FileInfo fileInfo = {filename, entry.path().string(), FileType::File};
    if (entry.is_regular_file()) {
      if (extensions.size() == 0) {
        files.push_back({filename, fileInfo});
      } else {
        for (auto &ext : extensions) {
          if (extension == ext) {
            files.push_back({filename, fileInfo});
            break;
          }
        }
      }
    } else if (entry.is_directory()) {
      fileInfo.type = FileType::Directory;
      files.push_back({filename, fileInfo});
    }
  }
  return files;
}

void FileDialog::createElements() {
  int buttonPadding = 10;
  int buttonWidth = (width - (buttonPadding * 3)) / 2;
  int buttonHeight = 30;
  int buttonX = x + buttonPadding;
  int buttonY = y + height - buttonHeight - buttonPadding;
  int pathBarHeight = 30;

  renderables.clear();
  eventHandlers.clear();

  // background
  Box *background = new Box(x, y, width, height, 0xFF000000);
  renderables.push_back(background);

  // Create the file dialog title.
  Text *titleText = new Text(title, x + 10, y + 10, 0xFFFFFFFF);
  renderables.push_back(titleText);

  // Create the file dialog list.
  std::vector<NamedItem> files = getFiles(initialPath, extensions);
  int listWidth = width - (buttonPadding * 2);
  int listHeight = height - (buttonPadding * 4) - buttonHeight - pathBarHeight;
  int listX = x + buttonPadding;
  int listY = y + buttonPadding * 2 + pathBarHeight;
  SelectionList *fileList = new SelectionList(files, listX, listY, listWidth, listHeight);
  renderables.push_back(fileList);
  eventHandlers.push_back(fileList);

  // Create the file dialog buttons.
  Button *openButton =
      new Button("Cancel", buttonX, buttonY, buttonWidth, buttonHeight, [this]() { this->callback({}); });
  renderables.push_back(openButton);
  eventHandlers.push_back(openButton);

  Button *saveButton = new Button(
      "Open", buttonX + buttonWidth + buttonPadding, buttonY, buttonWidth, buttonHeight, [this, fileList]() {
        std::optional<NamedItem> file = fileList->getSelectedItem();
        if (file.has_value()) {
          FileInfo fileInfo = std::any_cast<FileInfo>(file->value);
          printf("Selected file: %s\n", fileInfo.path.c_str());
          printf("Selected file type: %s\n", fileInfo.type == FileType::File ? "File" : "Directory");
          this->callback(file->name);
        } else {
          this->callback({});
        }
      });
  renderables.push_back(saveButton);
  eventHandlers.push_back(saveButton);

  // Create the file dialog path.
  EditableTextBox *pathText = new EditableTextBox("Path", initialPath, x + buttonPadding, y + buttonPadding,
                                                  width - (buttonPadding * 2), pathBarHeight, true);
  renderables.push_back(pathText);
  eventHandlers.push_back(pathText);
}

void FileDialog::setCallback(std::function<void(std::optional<std::string>)> callback) {
  this->callback = callback;
}

void FileDialog::close() {
  visible = false;
}