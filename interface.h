#include <string>

using SortingFunction = void (*)(int*);

struct TestFunction {
    SortingFunction func;
    std::string name;
};