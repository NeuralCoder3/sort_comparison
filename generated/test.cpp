#include <stdio.h>
#include <stdlib.h>
#include <array>
// #include "generated_test.h"
// #include "code/32/wrapper.h"
#include "code/generated.h"

int main() {
  using test_case_t = std::array<int , 3>;

  test_case_t test_cases[][2] = {
    /* actual      expected    */
    { { 1, 2, 3 }, { 1, 2, 3 } },
    { { 1, 3, 2 }, { 1, 2, 3 } },
    { { 2, 1, 3 }, { 1, 2, 3 } },
    { { 2, 3, 1 }, { 1, 2, 3 } },
    { { 3, 1, 2 }, { 1, 2, 3 } },
    { { 3, 2, 1 }, { 1, 2, 3 } },
    { { 1, 1, 2 }, { 1, 1, 2 } },
    { { 1, 2, 1 }, { 1, 1, 2 } },
    { { 2, 1, 1 }, { 1, 1, 2 } },
    { { 2, 2, 1 }, { 1, 2, 2 } },
    { { 2, 1, 2 }, { 1, 2, 2 } },
    { { 1, 2, 2 }, { 1, 2, 2 } },
    { { 1, 1, 1 }, { 1, 1, 1 } }
  };

  for (auto& test_case : test_cases) {
    test_case_t const& expected = test_case[1];
    test_case_t original = test_case[0];
    test_case_t actual = test_case[0];
    sort3_32_7_1(actual.data());
    if (actual[0] != expected[0] || actual[1] != expected[1] || actual[2] != expected[2]) {
      printf("FAIL on %d %d %d got %d %d %d, expected %d %d %d\n", 
        original[0], original[1], original[2],
        actual[0], actual[1], actual[2],
        expected[0], expected[1], expected[2]
      );
      return 1;
    }
  }

    return 0;
}