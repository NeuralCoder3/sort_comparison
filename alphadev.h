#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

#if defined(__clang__)

extern "C" void sort3_alphadev(int *buffer)
{
  asm volatile(
      "mov 0x4(%0), %%eax            \n"
      "mov 0x8(%0), %%ecx            \n"
      "cmp %%eax, %%ecx              \n"
      "mov %%eax, %%edx              \n"
      "cmovl %%ecx, %%edx            \n"
      "mov (%0), %%r8d               \n"
      "cmovg %%ecx, %%eax            \n"
      "cmp %%r8d, %%eax              \n"
      "mov %%r8d, %%ecx              \n"
      "cmovl %%eax, %%ecx            \n"
      "cmovle %%r8d, %%eax           \n"
      "mov %%eax, 0x8(%0)            \n"
      "cmp %%ecx, %%edx              \n"
      "cmovle %%edx, %%r8d           \n"
      "mov %%r8d, (%0)               \n"
      "cmovg %%edx, %%ecx            \n"
      "mov %%ecx, 0x4(%0)            \n"
      : "+r"(buffer)
      :
      : "eax", "ecx", "edx", "r8d", "memory");
}

#else

// Adapted from [1] for gcc. ("rd8" is replaced with "esi" for the compilation to succeed.)
extern "C" void sort3_alphadev(int *buffer)
{
  asm volatile(
      "mov 0x4(%0), %%eax            \n"
      "mov 0x8(%0), %%ecx            \n"
      "cmp %%eax, %%ecx              \n"
      "mov %%eax, %%edx              \n"
      "cmovl %%ecx, %%edx            \n"
      "mov (%0), %%esi               \n"
      "cmovg %%ecx, %%eax            \n"
      "cmp %%esi, %%eax              \n"
      "mov %%esi, %%ecx              \n"
      "cmovl %%eax, %%ecx            \n"
      "cmovle %%esi, %%eax           \n"
      "mov %%eax, 0x8(%0)            \n"
      "cmp %%ecx, %%edx              \n"
      "cmovle %%edx, %%esi           \n"
      "mov %%esi, (%0)               \n"
      "cmovg %%edx, %%ecx            \n"
      "mov %%ecx, 0x4(%0)            \n"
      : "+r"(buffer)
      :
      : "eax", "ecx", "edx", "esi", "memory");
}
#endif

#if defined(__clang__)

extern "C" void sort3_alphadev_reorder(int *buffer)
{
  asm volatile(
      "mov (%0), %%r8d               \n"
      "mov 0x4(%0), %%eax            \n"
      "mov 0x8(%0), %%ecx            \n"
      "cmp %%eax, %%ecx              \n"
      "mov %%eax, %%edx              \n"
      "cmovl %%ecx, %%edx            \n"
      "cmovg %%ecx, %%eax            \n"
      "cmp %%r8d, %%eax              \n"
      "mov %%r8d, %%ecx              \n"
      "cmovl %%eax, %%ecx            \n"
      "cmovle %%r8d, %%eax           \n"
      "cmp %%ecx, %%edx              \n"
      "cmovle %%edx, %%r8d           \n"
      "cmovg %%edx, %%ecx            \n"
      "mov %%r8d, (%0)               \n"
      "mov %%ecx, 0x4(%0)            \n"
      "mov %%eax, 0x8(%0)            \n"
      : "+r"(buffer)
      :
      : "eax", "ecx", "edx", "r8d", "memory");
}

#else

// Adapted from [1] for gcc. ("rd8" is replaced with "esi" for the compilation to succeed.)
extern "C" void sort3_alphadev_reorder(int *buffer)
{
  asm volatile(
      "mov (%0), %%esi               \n"
      "mov 0x4(%0), %%eax            \n"
      "mov 0x8(%0), %%ecx            \n"
      "cmp %%eax, %%ecx              \n"
      "mov %%eax, %%edx              \n"
      "cmovl %%ecx, %%edx            \n"
      "cmovg %%ecx, %%eax            \n"
      "cmp %%esi, %%eax              \n"
      "mov %%esi, %%ecx              \n"
      "cmovl %%eax, %%ecx            \n"
      "cmovle %%esi, %%eax           \n"
      "cmp %%ecx, %%edx              \n"
      "cmovle %%edx, %%esi           \n"
      "cmovg %%edx, %%ecx            \n"
      "mov %%esi, (%0)               \n"
      "mov %%ecx, 0x4(%0)            \n"
      "mov %%eax, 0x8(%0)            \n"
      : "+r"(buffer)
      :
      : "eax", "ecx", "edx", "esi", "memory");
}
#endif

// n = 4

#if defined(__clang__)

extern "C" void sort4_alphadev(int *buffer)
{
  asm volatile(
      "mov 0x8(%0), %%eax            \n"
      "mov (%0), %%ecx               \n"
      "mov 0x4(%0), %%edx            \n"
      "cmp %%eax, %%ecx              \n"
      "mov %%eax, %%r8d              \n"
      "cmovl %%ecx, %%r8d            \n"
      "cmovl %%eax, %%ecx            \n"
      "mov 0xc(%0), %%r9d            \n"
      "cmp %%r9d, %%edx              \n"
      "mov %%r9d, %%eax              \n"
      "cmovl %%edx, %%eax            \n"
      "cmovl %%r9d, %%edx            \n"
      "cmp %%eax, %%r8d              \n"
      "mov %%eax, %%r9d              \n"
      "cmovl %%r8d, %%r9d            \n"
      "cmovge %%r8d, %%eax           \n"
      "mov %%r9d, (%0)               \n"
      "cmp %%edx, %%ecx              \n"
      "mov %%edx, %%r8d              \n"
      "cmovl %%ecx, %%r8d            \n"
      "cmovge %%ecx, %%edx           \n"
      "mov %%edx, 0xc(%0)            \n"
      "cmp %%r8d, %%eax              \n"
      "mov %%r8d, %%ecx              \n"
      "cmovl %%eax, %%ecx            \n"
      "cmovge %%eax, %%r8d           \n"
      "mov %%r8d, 0x8(%0)            \n"
      "mov %%ecx, 0x4(%0)            \n"
      : "+r"(buffer)
      :
      : "eax", "ecx", "edx", "r8d", "r9d", "memory");
}

#else

// r8d -> esi
// r9d -> edi
extern "C" void sort4_alphadev(int *buffer)
{
  asm volatile(
      "mov 0x8(%0), %%eax            \n"
      "mov (%0), %%ecx               \n"
      "mov 0x4(%0), %%edx            \n"
      "cmp %%eax, %%ecx              \n"
      "mov %%eax, %%esi              \n"
      "cmovl %%ecx, %%esi            \n"
      "cmovl %%eax, %%ecx            \n"
      "mov 0xc(%0), %%edi            \n"
      "cmp %%edi, %%edx              \n"
      "mov %%edi, %%eax              \n"
      "cmovl %%edx, %%eax            \n"
      "cmovl %%edi, %%edx            \n"
      "cmp %%eax, %%esi              \n"
      "mov %%eax, %%edi              \n"
      "cmovl %%esi, %%edi            \n"
      "cmovge %%esi, %%eax           \n"
      "mov %%edi, (%0)               \n"
      "cmp %%edx, %%ecx              \n"
      "mov %%edx, %%esi              \n"
      "cmovl %%ecx, %%esi            \n"
      "cmovge %%ecx, %%edx           \n"
      "mov %%edx, 0xc(%0)            \n"
      "cmp %%esi, %%eax              \n"
      "mov %%esi, %%ecx              \n"
      "cmovl %%eax, %%ecx            \n"
      "cmovge %%eax, %%esi           \n"
      "mov %%esi, 0x8(%0)            \n"
      "mov %%ecx, 0x4(%0)            \n"
      : "+r"(buffer)
      :
      : "eax", "ecx", "edx", "esi", "edi", "memory");
}

#endif

#if defined(__clang__)

extern "C" void sort4_alphadev_reorder(int *buffer)
{
  asm volatile(
      "mov 0x8(%0), %%eax            \n"
      "mov (%0), %%ecx               \n"
      "mov 0x4(%0), %%edx            \n"
      "mov 0xc(%0), %%r9d            \n"
      "cmp %%eax, %%ecx              \n"
      "mov %%eax, %%r8d              \n"
      "cmovl %%ecx, %%r8d            \n"
      "cmovl %%eax, %%ecx            \n"
      "cmp %%r9d, %%edx              \n"
      "mov %%r9d, %%eax              \n"
      "cmovl %%edx, %%eax            \n"
      "cmovl %%r9d, %%edx            \n"
      "cmp %%eax, %%r8d              \n"
      "mov %%eax, %%r9d              \n"
      "cmovl %%r8d, %%r9d            \n"
      "cmovge %%r8d, %%eax           \n"
      "cmp %%edx, %%ecx              \n"
      "mov %%edx, %%r8d              \n"
      "cmovl %%ecx, %%r8d            \n"
      "cmovge %%ecx, %%edx           \n"
      "cmp %%r8d, %%eax              \n"
      "mov %%r8d, %%ecx              \n"
      "cmovl %%eax, %%ecx            \n"
      "cmovge %%eax, %%r8d           \n"
      "mov %%r9d, (%0)               \n"
      "mov %%edx, 0xc(%0)            \n"
      "mov %%r8d, 0x8(%0)            \n"
      "mov %%ecx, 0x4(%0)            \n"
      : "+r"(buffer)
      :
      : "eax", "ecx", "edx", "r8d", "r9d", "memory");
}

#else

// r8d -> esi
// r9d -> edi
extern "C" void sort4_alphadev_reorder(int *buffer)
{
  asm volatile(
      "mov 0x8(%0), %%eax            \n"
      "mov (%0), %%ecx               \n"
      "mov 0x4(%0), %%edx            \n"
      "mov 0xc(%0), %%edi            \n"
      "cmp %%eax, %%ecx              \n"
      "mov %%eax, %%esi              \n"
      "cmovl %%ecx, %%esi            \n"
      "cmovl %%eax, %%ecx            \n"
      "cmp %%edi, %%edx              \n"
      "mov %%edi, %%eax              \n"
      "cmovl %%edx, %%eax            \n"
      "cmovl %%edi, %%edx            \n"
      "cmp %%eax, %%esi              \n"
      "mov %%eax, %%edi              \n"
      "cmovl %%esi, %%edi            \n"
      "cmovge %%esi, %%eax           \n"
      "cmp %%edx, %%ecx              \n"
      "mov %%edx, %%esi              \n"
      "cmovl %%ecx, %%esi            \n"
      "cmovge %%ecx, %%edx           \n"
      "cmp %%esi, %%eax              \n"
      "mov %%esi, %%ecx              \n"
      "cmovl %%eax, %%ecx            \n"
      "cmovge %%eax, %%esi           \n"
      "mov %%edi, (%0)               \n"
      "mov %%edx, 0xc(%0)            \n"
      "mov %%esi, 0x8(%0)            \n"
      "mov %%ecx, 0x4(%0)            \n"
      : "+r"(buffer)
      :
      : "eax", "ecx", "edx", "esi", "edi", "memory");
}

#endif



void sort5_alphadev(int* buffer) {
  asm volatile(
      "mov (%0), %%eax               \n"
      "mov 0x4(%0), %%ecx            \n"
      "cmp %%eax, %%ecx              \n"
      "mov %%eax, %%edx              \n"
      "cmovl %%ecx, %%edx            \n"
      "cmovg %%ecx, %%eax            \n"
      "mov 0xc(%0), %%r8d            \n"
      "mov 0x10(%0), %%ecx           \n"
      "cmp %%r8d, %%ecx              \n"
      "mov %%r8d, %%r9d              \n"
      "cmovl %%ecx, %%r9d            \n"
      "cmovg %%ecx, %%r8d            \n"
      "mov 0x8(%0), %%r10d           \n"
      "cmp %%r10d, %%r8d             \n"
      "mov %%r10d, %%ecx             \n"
      "cmovl %%r8d, %%ecx            \n"
      "cmovle %%r10d, %%r8d          \n"
      "cmp %%ecx, %%r9d              \n"
      "cmovle %%r9d, %%r10d          \n"
      "cmovg %%r9d, %%ecx            \n"
      "cmp %%eax, %%r8d              \n"
      "mov %%eax, %%r9d              \n"
      "cmovl %%r8d, %%r9d            \n"
      "cmovle %%eax, %%r8d           \n"
      "cmp %%edx, %%ecx              \n"
      "mov %%edx, %%eax              \n"
      "cmovl %%ecx, %%eax            \n"
      "cmovle %%edx, %%ecx           \n"
      "mov %%r8d, 0x10(%0)           \n"
      "cmp %%eax, %%r10d             \n"
      "cmovle %%r10d, %%edx          \n"
      "mov %%edx, (%0)               \n"
      "cmovg %%r10d, %%eax           \n"
      "cmp %%r9d, %%ecx              \n"
      "mov %%r9d, %%r8d              \n"
      "cmovl %%ecx, %%r8d            \n"
      "cmovle %%r9d, %%ecx           \n"
      "mov %%ecx, 0xc(%0)            \n"
      "cmp %%r8d, %%eax              \n"
      "cmovle %%eax, %%r9d           \n"
      "mov %%r9d, 0x4(%0)            \n"
      "cmovg %%eax, %%r8d            \n"
      "mov %%r8d, 0x8(%0)            \n"
      : "+r"(buffer)
      :
      : "eax", "ecx", "edx", "r8d", "r9d", "r10d", "memory");
}