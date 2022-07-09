#pragma once

#include <functional>
#include <string>

struct DropdownOption {
  std::string name;
  std::function<void()> value;
};
