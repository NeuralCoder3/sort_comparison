#include "interface.h"
#include "alphadev.h"
#include "cassioneri.h"
#include "custom.h"
#include "mimicry.h"
#include "rust.h"
#include "generated/code5/generated.h"
// #include "generated/selected.h"

#include <array>

// n = 5

TestFunction predefined_functions[] = {

    // {sort4_default, "default"},
    // {sort4_default_size, "default_size"},
    // {sort4_std, "std"},
    // {sort4_branchless, "branchless"},
    // {sort4_swap, "swap"},

    // {sort4_mimicry_ms, "mimicry_ms"},
    // {sort4_mimicry_mv, "mimicry_mv"},

    {sort5_alphadev, "alphadev"},
};

std::vector<TestFunction> MakeFunctions()
{
    std::vector<TestFunction> functions;
    for (auto &func : predefined_functions)
    {
        functions.push_back(func);
    }
    for (auto &func : gen_functions)
    {
        functions.push_back(func);
    }
    return functions;
}

std::vector<TestFunction> functions = MakeFunctions();
