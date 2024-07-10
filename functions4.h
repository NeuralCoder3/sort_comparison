#include "interface.h"
#include "alphadev.h"
#include "cassioneri.h"
#include "custom.h"
#include "mimicry.h"
#include "rust.h"
#include "generated/code4/generated.h"
// #include "generated/selected.h"

#include <array>

// n = 4

TestFunction predefined_functions[] = {

    {sort4_default, "default"},
    {sort4_default_size, "default_size"},
    {sort4_std, "std"},
    {sort4_branchless, "branchless"},
    // { sort4_branchless_size, "branchless_size" },
    {sort4_swap, "swap"},
    // { sort4_swap_inline,     "swap_inline" },

    // { sort3_mimicry_mu, "sort3_mimicry_mu" },
    {sort4_mimicry_ms, "mimicry_ms"},
    {sort4_mimicry_mv, "mimicry_mv"},

    // { sort3_rust_std, "rust_std" },
    // { sort3_rust_swap_inline_annotated, "rust_swap_inline_annotated" },
    // { sort3_rust_swap_inline_raw, "rust_swap_inline_raw" },
    // { sort3_rust_branchless_min_annotated, "rust_branchless_min_annotated" },
    // { sort3_rust_branchless_annotated, "rust_branchless_annotated" },

    {sort4_alphadev, "alphadev"},
    {sort4_alphadev_reorder, "alphadev_reorder"},
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
