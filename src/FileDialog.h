#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <filesystem>
#include <future>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "EventHandler.h"
#include "NamedItem.h"
#include "Object.h"
#include "Renderable.h"
#include "gui_elements/gui_elements.h"

enum class FileDialogMode { Open, Save };
enum class FileType { File, Directory };
struct FileInfo {
  std::string filename;
  std::string path;
  FileType type;
};

class FileDialog : public Renderable, public EventHandler, virtual public Object {
 private:
  FileDialogMode mode;
  std::string title, filePath, initialPath;
  std::vector<std::string> extensions;
  std::vector<Renderable *> renderables;
  std::vector<EventHandler *> eventHandlers;
  std::function<void(std::optional<std::string>)> callback;
  SelectionList *fileList;
  EditableTextBox *pathText;

  bool visible;

  void createElements();

 public:
  FileDialog(FileDialogMode mode, std::string title, std::string initialPath,
             std::vector<std::string> extensions, int x, int y, int width, int height);
  void showFileDialog();
  void close();
  void handleEvent(SDL_Event *event);
  void render(SDL_Renderer *renderer);
  void setCallback(std::function<void(std::optional<std::string>)> callback);
  void setFilePath(std::string path);
};