#pragma once

#include <any>
#include <string>

struct NamedItem {
  std::string name;
  std::any value;
};