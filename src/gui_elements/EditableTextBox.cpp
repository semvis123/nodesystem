#include "EditableTextBox.h"

EditableTextBox::EditableTextBox(std::string placeholder, std::string defaultText, int x, int y, int width,
                                 int height, bool renderCursor) {
  this->placeholder = placeholder;
  this->text = defaultText;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->hasFocus = false;
  this->renderCursor = renderCursor;
  this->cursorPosition = 0;
}

void EditableTextBox::render(SDL_Renderer *renderer) {
  int32_t bgColor = BG_COLOR;
  int32_t borderColor = BORDER_COLOR;

  if (hasFocus) {
    bgColor = FOCUS_BG_COLOR;
    borderColor = FOCUS_BORDER_COLOR;
  } else if (isHovering) {
    bgColor = HOVER_BG_COLOR;
    borderColor = HOVER_BORDER_COLOR;
  }

  // background
  boxColor(renderer, x, y, x + width, y + height, bgColor);
  rectangleColor(renderer, x, y, x + width, y + height, borderColor);

  // cut off text if it is too long, still need to figure out how to do this properly
  int maxTextLength = (width - PADDING * 2) / TEXT_WIDTH;
  std::string renderText = text;
  if (text.length() > maxTextLength) {
    renderText = text.substr(textRenderOffset, fmin(maxTextLength, text.length() - textRenderOffset));
  }

  // draw text or placeholder
  if (renderText.length() > 0) {
    stringColor(renderer, x + PADDING, y + PADDING, renderText.c_str(), TEXT_COLOR);
  } else {
    stringColor(renderer, x + PADDING, y + PADDING, placeholder.c_str(), PLACEHOLDER_COLOR);
  }

  // draw selection
  if (selectionStart != selectionEnd && selectionStart != cursorPosition && hasFocus) {
    int selectionX = x + PADDING + TEXT_WIDTH * (selectionStart - textRenderOffset);
    int selectionWidth = TEXT_WIDTH * (selectionEnd - selectionStart);

    // ensure selection is not outside of the text box
    if (selectionX < x + PADDING) {
      selectionWidth -= (x + PADDING - selectionX);
      selectionX = x + PADDING;
    }

    if (selectionX + selectionWidth > x + width - PADDING) {
      selectionWidth = x + width - PADDING - selectionX;
    }

    boxColor(renderer, selectionX, y + PADDING, selectionX + selectionWidth, y + height - PADDING,
             0x5500FF00);
  }

  // draw cursor
  if (renderCursor && hasFocus && (SDL_GetTicks() % 1000 < 500 || isPressed)) {
    int cursorX = x + PADDING + TEXT_WIDTH * (cursorPosition - textRenderOffset);
    int cursorWidth = 2;
    boxColor(renderer, cursorX, y + PADDING, cursorX + cursorWidth, y + height - PADDING, CURSOR_COLOR);
  }
}

void EditableTextBox::handleEvent(SDL_Event *event) {
  int maxTextLength = (width - PADDING * 2) / TEXT_WIDTH;

  switch (event->type) {
    case (SDL_KEYDOWN): {
      switch (event->key.keysym.sym) {
        case (SDLK_BACKSPACE): {
          // remove character at cursor position (or everything left of it when modifier is held),
          // and remove selection if any
          int temp = cursorPosition;
          int start = fmax(cursorPosition - 1, 0);
          int end = cursorPosition;

          if (selectionStart != selectionEnd) {
            start = fmin(selectionStart, selectionEnd);
            end = fmax(selectionStart, selectionEnd);
          }

          if (event->key.keysym.mod & MODIFIER_KEY) {
            start = 0;
          }

          text.erase(start, end - start);
          cursorPosition = start;
          selectionStart = selectionEnd = cursorPosition;

          //  if cursor is off screen, move textRenderOffset to make it on screen
          textRenderOffset = fmax(textRenderOffset - (temp - cursorPosition), 0);

          break;
        }
        case (SDLK_DELETE): {
          // remove character after cursor position (or everything right of it when modifier is
          // held), and remove selection if any
          int temp = cursorPosition;
          int start = cursorPosition;
          int end = fmin(cursorPosition + 1, text.length());

          if (selectionStart != selectionEnd) {
            start = fmin(selectionStart, selectionEnd);
            end = fmax(selectionStart, selectionEnd);
          } else {
            selectionStart = selectionEnd = cursorPosition;
          }

          if (event->key.keysym.mod & MODIFIER_KEY) {
            end = text.length();
          }

          text.erase(start, end - start);
          cursorPosition = fmax(0, fmin(fmin(selectionStart, selectionEnd), text.length()));
          selectionStart = selectionEnd = cursorPosition;

          // if cursor is off screen, move textRenderOffset to make it on screen
          textRenderOffset = fmax(textRenderOffset - (temp - cursorPosition), 0);

          break;
        }
        case (SDLK_LEFT): {
          // move cursor left (or to the beginning if modifier is held), and extend selection if
          // shift is held
          int temp = cursorPosition;
          if (cursorPosition > 0) {
            cursorPosition--;
          }

          if (event->key.keysym.mod & MODIFIER_KEY) {
            cursorPosition = 0;
          }

          if (event->key.keysym.mod & KMOD_SHIFT) {
            if (selectionStart == selectionEnd) {
              selectionStart = temp;
            }
            selectionEnd = cursorPosition;
          } else {
            selectionStart = selectionEnd;
          }

          int maxTextLength = (width - PADDING * 2) / TEXT_WIDTH;
          if (cursorPosition < textRenderOffset) {
            textRenderOffset = fmax(textRenderOffset - (temp - cursorPosition), 0);
          }

          break;
        }
        case (SDLK_RIGHT): {
          // move cursor right (or to the end if modifier is held), and extend selection if shift is
          // held
          int temp = cursorPosition;
          if (cursorPosition < text.length()) {
            cursorPosition++;
          }

          if (event->key.keysym.mod & MODIFIER_KEY) {
            cursorPosition = text.length();
          }

          // if cursor is off screen, move textRenderOffset to make it on screen
          if (cursorPosition > textRenderOffset + maxTextLength) {
            textRenderOffset = fmin(textRenderOffset + (cursorPosition - temp), text.length());
          }

          if (event->key.keysym.mod & KMOD_SHIFT) {
            if (selectionStart == selectionEnd) {
              selectionStart = temp;
            }
            selectionEnd = cursorPosition;
          } else {
            selectionStart = selectionEnd;
          }
          break;
        }
        case (SDLK_HOME): {
          // move to start of text, and extend selection if shift is held
          if (event->key.keysym.mod & KMOD_SHIFT) {
            if (selectionStart == selectionEnd) {
              selectionStart = cursorPosition;
            }
            selectionEnd = 0;
          } else {
            selectionStart = selectionEnd;
          }
          cursorPosition = 0;
          break;
        }
        case (SDLK_END): {
          // move to end of text, and extend selection if shift is held
          if (event->key.keysym.mod & KMOD_SHIFT) {
            if (selectionStart == selectionEnd) {
              selectionStart = cursorPosition;
            }
            selectionEnd = text.length();
          } else {
            selectionStart = selectionEnd;
          }
          cursorPosition = text.length();

          // if cursor is off screen, move textRenderOffset to make it on screen
          if (cursorPosition - textRenderOffset >= maxTextLength) {
            textRenderOffset = cursorPosition - maxTextLength + 1;
          }

          break;
        }
        case (SDLK_c): {
          // copy selection to clipboard
          if (event->key.keysym.mod & MODIFIER_KEY) {
            if (selectionStart != selectionEnd) {
              SDL_SetClipboardText(
                  text.substr(fmin(selectionStart, selectionEnd), abs(selectionStart - selectionEnd))
                      .c_str());
            }
          }
          break;
        }
        case (SDLK_v): {
          // paste from clipboard, remove selection if any
          if (event->key.keysym.mod & MODIFIER_KEY) {
            char *clipboardText = SDL_GetClipboardText();
            if (selectionStart != selectionEnd) {
              text.erase(text.begin() + selectionStart, text.begin() + selectionEnd);
              cursorPosition = selectionStart;
              selectionStart = selectionEnd = 0;
            }
            text.insert(cursorPosition, clipboardText);
            cursorPosition += strlen(clipboardText);
            SDL_free(clipboardText);
          }
          break;
        }
        default:
          break;
      }
      break;
    }
    case (SDL_TEXTINPUT): {
      // add text to text, and move cursor right (replace selection if any)
      if (selectionEnd != selectionStart) {
        text.erase(fmin(selectionStart, selectionEnd), abs(selectionEnd - selectionStart));
        cursorPosition = fmin(selectionStart, selectionEnd);
        selectionStart = selectionEnd = cursorPosition;
      }

      text.insert(cursorPosition, event->text.text);
      cursorPosition += strlen(event->text.text);

      // if cursor is off screen, move textRenderOffset to make it on screen
      if (cursorPosition - textRenderOffset >= maxTextLength) {
        textRenderOffset = cursorPosition - maxTextLength + 1;
      }

      break;
    }
    case (SDL_MOUSEBUTTONDOWN): {
      // set focus and place cursor at mouse position
      if (inside(event->button.x, event->button.y)) {
        selectionStart = findCursorTextPosition(event->button.x);
        selectionEnd = selectionStart;
        isPressed = true;

      } else {
        hasFocus = false;
      }
      break;
    }
    case (SDL_MOUSEBUTTONUP): {
      // handle selection, and remove focus if mouse is outside of box
      bool oldFocus = hasFocus;
      hasFocus = inside(event->button.x, event->button.y);
      if (hasFocus) {
        cursorPosition = findCursorTextPosition(event->button.x);
        isPressed = false;
      }
      if (oldFocus != hasFocus) {
        selectionStart = selectionEnd = cursorPosition;
      }

      // select word on double click
      if (lastClick + DOUBLE_CLICK_TIME > SDL_GetTicks() && lastClick != 0) {
        selectionStart = selectionEnd = cursorPosition;
        while (selectionStart > 0 && isalnum(text[selectionStart - 1])) {
          selectionStart--;
        }
        while (selectionEnd < text.length() && isalnum(text[selectionEnd])) {
          selectionEnd++;
        }
        cursorPosition = selectionEnd;
      }
      lastClick = SDL_GetTicks();

      break;
    }
    case (SDL_MOUSEMOTION): {
      // set hover, and update selection if mouse is pressed
      isHovering = inside(event->motion.x, event->motion.y);
      if (isHovering && isPressed) {
        cursorPosition = findCursorTextPosition(event->motion.x);
        selectionEnd = cursorPosition;
      }
    }
  }
}

void EditableTextBox::setText(std::string text) {
  this->text = text;
}

std::string EditableTextBox::getText() {
  return text;
}

bool EditableTextBox::inside(int x, int y) {
  return x >= this->x && x <= this->x + this->width && y >= this->y && y <= this->y + this->height;
}

int EditableTextBox::findCursorTextPosition(int x) {
  int maxTextLength = (width - PADDING * 2) / TEXT_WIDTH;
  int cursorPosition = 0;
  for (int i = 0; i < text.length() + 1; i++) {
    if (x >= this->x + PADDING + TEXT_WIDTH * i) {
      cursorPosition = i;
    }
  }
  return fmin(cursorPosition + textRenderOffset, fmin(maxTextLength + textRenderOffset, text.length()));
}