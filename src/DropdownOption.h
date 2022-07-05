#include <string>
#include <functional>

struct DropdownOption
{
    std::string name;
    std::function<void()> value;
};
