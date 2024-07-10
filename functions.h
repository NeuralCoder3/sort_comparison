#include "interface.h"
#include "alphadev.h"
#include "cassioneri.h"
#include "custom.h"
#include "mimicry.h"
#include "rust.h"
#include "generated/code/generated.h"
// #include "generated/selected.h"

#include <array>

TestFunction predefined_functions[] = {

    {sort3_cassioneri_14, "cassioneri_14"},
    {sort3_cassioneri_15, "cassioneri_15"},
    {sort3_cassioneri_15_v2, "cassioneri_15_v2"},
    {sort3_cassioneri_faster, "cassioneri_faster"},

    {sort3_default, "default"},
    {sort3_default_size, "default_size"},
    {sort3_std, "std"},
    {sort3_branchless, "branchless"},
    {sort3_branchless_size, "branchless_size"},
    {sort3_swap, "swap"},
    {sort3_swap_inline, "swap_inline"},
    {sort3_sorting_network, "sorting_network"},
    {sort3_xmm, "pminsd_xmm"},

    // { sort3_mimicry_mu, "sort3_mimicry_mu" },
    {sort3_mimicry_ms, "mimicry_ms"},
    {sort3_mimicry_mv, "mimicry_mv"},
    {sort3_mimicry_std, "mimicry_std"},

    {sort3_rust_std, "rust_std"},
    {sort3_rust_swap_inline_annotated, "rust_swap_inline_annotated"},
    {sort3_rust_swap_inline_raw, "rust_swap_inline_raw"},
    {sort3_rust_branchless_min_annotated, "rust_branchless_min_annotated"},
    {sort3_rust_branchless_annotated, "rust_branchless_annotated"},

    {sort3_alphadev, "alphadev"},
    {sort3_alphadev_reorder, "alphadev_reorder"},
    // { sort3_32_7_1, "32_7_1" },
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

// add gen_functions to functions
