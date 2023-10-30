#if defined(__clang__)

// mov 4 1
// cmp 2 4
// cmovg 4 2
// cmovg 2 1
// mov 1 2
// cmp 2 3
// cmovl 2 3
// cmovg 1 3
// cmp 3 4
// cmovl 3 4
// cmovg 2 4

// 1 = r8d
// 2 = eax
// 3 = ecx
// 4 = edx

// mov is swapped (mov from to -- above it is mov to from)


extern "C" void sort3_32_7_1(int* buffer) {
  asm volatile(
        #include "code/32/7/1.c"
        // "mov (%0), %%r8d        \n"
        // "mov 0x4(%0), %%eax     \n"
        // "mov 0x8(%0), %%ecx     \n"
        // "mov %%r8d, %%edx       \n"
        // "cmp %%edx, %%eax       \n"
        // "cmovg %%eax, %%edx     \n"
        // "cmovg %%r8d, %%eax     \n"
        // "mov %%eax, %%r8d       \n"
        // "cmp %%ecx, %%eax       \n"
        // "cmovl %%ecx, %%eax     \n"
        // "cmovg %%ecx, %%r8d     \n"
        // "cmp %%edx, %%ecx       \n"
        // "cmovl %%edx, %%ecx     \n"
        // "cmovg %%edx, %%eax     \n"
        // "mov %%r8d, (%0)        \n"
        // "mov %%eax, 0x4(%0)     \n"
        // "mov %%ecx, 0x8(%0)     \n"
      : "+r"(buffer)
      :
      : "eax", "ecx", "edx", "r8d", "memory");

//   asm volatile(
//       "mov (%0), %%r8d               \n"
//       "mov 0x4(%0), %%eax            \n"
//       "mov 0x8(%0), %%ecx            \n"
//       "cmp %%eax, %%ecx              \n"
//       "mov %%eax, %%edx              \n"
//       "cmovl %%ecx, %%edx            \n"
//       "cmovg %%ecx, %%eax            \n"
//       "cmp %%r8d, %%eax              \n"
//       "mov %%r8d, %%ecx              \n"
//       "cmovl %%eax, %%ecx            \n"
//       "cmovle %%r8d, %%eax           \n"
//       "cmp %%ecx, %%edx              \n"
//       "cmovle %%edx, %%r8d           \n"
//       "cmovg %%edx, %%ecx            \n"
//       "mov %%r8d, (%0)               \n"
//       "mov %%ecx, 0x4(%0)            \n"
//       "mov %%eax, 0x8(%0)            \n"
//       : "+r"(buffer)
//       :
//       : "eax", "ecx", "edx", "r8d", "memory");
}

#else

extern "C" void sort3_32_7_1(int* buffer) {
}
#endif
