#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <string>

#include "../Positional.h"
#include "../Sizable.h"
#include "EventHandler.h"
#include "Renderable.h"

// ctrl on windows, cmd on mac
#ifdef __APPLE__
#define MODIFIER_KEY KMOD_GUI
#else
#define MODIFIER_KEY KMOD_CTRL
#endif

class EditableTextBox : public Renderable, public EventHandler, virtual Positional, virtual Sizable {
 private:
  std::string placeholder, text;
  bool hasFocus, renderCursor, isHovering, isPressed;
  int cursorPosition, textRenderOffset = 0, selectionStart, selectionEnd, lastClick;

  const int PADDING = 10, TEXT_WIDTH = 8, DOUBLE_CLICK_TIME = 500;
  const uint32_t BG_COLOR = 0xFF244622, BORDER_COLOR = 0xFF000000, TEXT_COLOR = 0xFFFFFFFF,
                 PLACEHOLDER_COLOR = 0xFFCCCCCC, FOCUS_BORDER_COLOR = 0xFF339930, FOCUS_BG_COLOR = 0xFF325822,
                 HOVER_BORDER_COLOR = 0xFF000000, HOVER_BG_COLOR = 0xFF143612, CURSOR_COLOR = 0xFF000000;
  bool inside(int x, int y);
  int findCursorTextPosition(int x);

 public:
  EditableTextBox(std::string placeholder, std::string defaultText, int x, int y, int width, int height,
                  bool renderCursor = true);
  void render(SDL_Renderer *renderer);
  void handleEvent(SDL_Event *event);
  void setText(std::string text);
  std::string getText();
};
