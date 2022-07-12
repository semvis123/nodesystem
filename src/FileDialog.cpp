#include "FileDialog.h"

FileDialog::FileDialog(FileDialogMode mode, std::string title, std::string initialPath,
                       std::vector<std::string> extensions, int x, int y, int width, int height) {
  this->mode = mode;
  this->title = title;
  this->filePath = initialPath;
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

std::vector<NamedItem> getFiles(std::string path, std::vector<std::string> extensions,
                                bool showParentDirectory = true) {
  // Get all filenames in the given path with the given extensions if any
  std::vector<NamedItem> files;
  std::string removed;
  std::filesystem::path fsPath = path;
  if ((path.back() == std::filesystem::path::preferred_separator) && (fsPath.root_path() != fsPath)) {
    removed = path.back();
    path.pop_back();
  }

  std::string parentDirectory = std::filesystem::path(path).parent_path().string();

  for (std::string *p : {&path, &parentDirectory}) {
    if (p->back() != std::filesystem::path::preferred_separator) {
      p->push_back(std::filesystem::path::preferred_separator);
    }
  }

  if (parentDirectory != path && showParentDirectory) {
    FileInfo parentDirectoryFileInfo = {"..", parentDirectory, FileType::Directory};
    files.push_back(NamedItem({"..", parentDirectoryFileInfo}));
  }

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
      fileInfo.path += std::filesystem::path::preferred_separator;
      filename += std::filesystem::path::preferred_separator;
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
  fileList = new SelectionList(files, listX, listY, listWidth, listHeight);
  renderables.push_back(fileList);
  eventHandlers.push_back(fileList);

  fileList->setDoubleClickCallback([this](NamedItem item) {
    FileInfo fileInfo = std::any_cast<FileInfo>(item.value);
    if (fileInfo.type == FileType::Directory) {
      setFilePath(fileInfo.path);
    } else {
      if (callback) {
        callback(fileInfo.path);
      }
    }
  });

  // Create the file dialog buttons.
  Button *openButton =
      new Button("Cancel", buttonX, buttonY, buttonWidth, buttonHeight, [this]() { this->callback({}); });
  renderables.push_back(openButton);
  eventHandlers.push_back(openButton);

  Button *saveButton =
      new Button("Open", buttonX + buttonWidth + buttonPadding, buttonY, buttonWidth, buttonHeight, [this]() {
        std::optional<NamedItem> file = fileList->getSelectedItem();
        if (file.has_value()) {
          FileInfo fileInfo = std::any_cast<FileInfo>(file->value);

          printf("Selected file: %s\n", fileInfo.path.c_str());
          printf("Selected file type: %s\n", fileInfo.type == FileType::File ? "File" : "Directory");
          if (fileInfo.type == FileType::Directory) {
            setFilePath(fileInfo.path);
            return;
          }
          this->callback(file->name);
        } else {
          this->callback({});
        }
      });
  renderables.push_back(saveButton);
  eventHandlers.push_back(saveButton);

  // Create the file dialog path.
  pathText = new EditableTextBox("Path", initialPath, x + buttonPadding, y + buttonPadding,
                                 width - (buttonPadding * 2), pathBarHeight, true);
  renderables.push_back(pathText);
  eventHandlers.push_back(pathText);
}

void FileDialog::setFilePath(std::string path) {
  filePath = path;
  fileList->setItems(getFiles(filePath, extensions));
  pathText->setText(filePath);
}

void FileDialog::setCallback(std::function<void(std::optional<std::string>)> callback) {
  this->callback = callback;
}

void FileDialog::close() {
  visible = false;
}