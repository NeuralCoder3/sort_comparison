#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

// Sort3mu - sort 3 uint32_t values pointed to by buffer
uint8_t shuf3mu0[] = {
	8, 4, 8, 0, 4, 0};

uint8_t shuf3mu1[] = {
	4, 8, 0, 8, 0, 4};

extern "C" void sort3_mimicry_mu(uint32_t *buffer)
{
	asm volatile(
		"mov (%0), %%eax                        \n"
		"mov 4(%0), %%edx                       \n"
		"cmp %%eax, %%edx                       \n"
		"sbb %%rbx, %%rbx                       \n"
		"mov 8(%0), %%ecx                       \n"
		"cmp %%eax, %%ecx                       \n"
		"sbb %%r8, %%r8                         \n"
		"cmp %%ecx, %%edx                       \n"
		"adc $0, %%r8                           \n"
		"movzb shuf3mu0+3(%%rbx,%%r8,2), %%r9   \n"
		"mov %%ecx, 4(%0,%%r8,4)                \n"
		"movzb shuf3mu1+3(%%rbx,%%r8,2), %%rcx  \n"
		"mov %%eax, (%0,%%r9)                   \n"
		"mov %%edx, (%0,%%rcx)                  \n"
		: "+r"(buffer)
		:
		: "eax", "edx", "rbx", "rcx", "r8", "r9", "memory");
}

// Sort3ms - sort 3 int32_t values pointed to by buffer;
// reuses the write shuffle vectors from Sort3mu
extern "C" void sort3_mimicry_ms(int32_t *buffer)
{
	asm volatile(
		"mov (%0), %%eax \n"
		"mov 4(%0), %%edx \n"
		"lea 0x80000000(%%eax), %%r10d \n"
		"lea 0x80000000(%%edx), %%r11d \n"
		"cmp %%r10d, %%r11d \n"
		"sbb %%rbx, %%rbx \n" // range -1..0
		"mov 8(%0), %%ecx \n"
		"lea 0x80000000(%%ecx), %%r12d \n"
		"cmp %%r10d, %%r12d \n"
		"sbb %%r8, %%r8 \n"
		"cmp %%r12d, %%r11d \n"
		"adc $0, %%r8 \n" // range -1..1
		"movzb shuf3mu0+3(%%rbx,%%r8,2), %%r9 \n"
		"mov %%ecx, 4(%0,%%r8,4) \n"
		"movzb shuf3mu1+3(%%rbx,%%r8,2), %%rcx \n"
		"mov %%eax, (%0,%%r9) \n"
		"mov %%edx, (%0,%%rcx) \n"
		: "+r"(buffer)
		:
		: "eax", "edx", "rbx", "rcx", "r8", "r9", "r10", "r11", "r12", "memory");
}

// Sort3mv - sort 3 int32_t values pointed to by buffer, using SIMD
// instructions

// first three elements mask for masked move (vpmaskmovd) instruction
int32_t mask3[] = {-1, -1, -1, 0};

// byte shuffles representing 32-bit word positions
const uint32_t p0 = 0x03020100;
const uint32_t p1 = 0x07060504;
const uint32_t p2 = 0x0b0a0908;
const uint32_t p3 = 0x0f0e0d0c;
const uint32_t invp = 0xffffffff;

/* Sort3 shuffles
0b000 -> 0 1 2
0b001 -> 1 2 0
0b010 -> 2 0 1
0b011 -> 2 1 0
0b100 -> 0 1 2
0b101 -> 1 0 2
0b110 -> 0 2 1
*/

// In the shuffle definition, if we want to retain the last (unsorted) word,
// the last element needs to be set to p3, if we want to clear it to 0, the
// last element must be invp
uint32_t vshuf3[7 * 4] = {
	p0, p1, p2, p3,
	p1, p2, p0, p3,
	p2, p0, p1, p3,
	p2, p1, p0, p3,
	p0, p1, p2, p3,
	p1, p0, p2, p3,
	p0, p2, p1, p3};

extern "C" void sort3_mimicry_mv(int32_t *buffer)
{
	asm volatile(
		"vmovdqa mask3, %%xmm1                 \n"
		"vpmaskmovd (%0), %%xmm1, %%xmm2       \n"
		"vpshufd $0xc9, %%xmm2, %%xmm3         \n" // shuffle 0b1100_1001 -> 1 2 0 3
		"vpcmpgtd %%xmm3, %%xmm2, %%xmm3       \n"
		"movmskps %%xmm3, %%eax                \n"
		"shl $4, %%eax                         \n"
		"vpshufb vshuf3(%%eax), %%xmm2, %%xmm2 \n"
		"vpextrq $0, %%xmm2, (%0)              \n"
		"vpextrd $2, %%xmm2, 8(%0)             \n"
		: "+r"(buffer)
		:
		: "eax", "xmm1", "xmm2", "xmm3", "memory");
}

extern "C" void sort3_mimicry_std(int32_t *buffer)
{
	std::sort(buffer, buffer + 3);
}

// n = 4

// Sort4mu - sort 4 uint32_t values pointed to by buffer
int8_t shuf4mu0[] = {
	// src position 0
	2, 3, 3, 1, 1, 2, -1, -1, // last elem to pos 0
	2, 3, 3, 0, 0, 2, -1, -1, // last elem to pos 1
	1, 3, 3, 0, 0, 1, -1, -1, // last elem to pos 2
	1, 2, 2, 0, 0, 1, -1, -1  // last elem to pos 3
};

int8_t shuf4mu1[] = {
	// src position 1
	1, 2, 1, 3, 2, 3, -1, -1, // last elem to pos 0
	0, 2, 0, 3, 2, 3, -1, -1, // last elem to pos 1
	0, 1, 0, 3, 1, 3, -1, -1, // last elem to pos 2
	0, 1, 0, 2, 1, 2, -1, -1  // last elem to pos 3
};

int8_t shuf4mu2[] = {
	// src position 2
	3, 1, 2, 2, 3, 1, -1, -1, // last elem to pos 0
	3, 0, 2, 2, 3, 0, -1, -1, // last elem to pos 1
	3, 0, 1, 1, 3, 0, -1, -1, // last elem to pos 2
	2, 0, 1, 1, 2, 0, -1, -1  // last elem to pos 3
};

void sort4_mimicry_mu(uint32_t *buffer)
{
	asm volatile(
		"mov (%0), %%eax                        \n"
		"mov 4(%0), %%edx                       \n"
		"cmp %%eax, %%edx                       \n"
		"sbb %%rbx, %%rbx                       \n"
		"mov 8(%0), %%ecx                       \n"
		"mov 12(%0), %%r10d                     \n"
		"cmp %%eax, %%ecx                       \n"
		"adc %%rbx, %%rbx                       \n"
		"cmp %%eax, %%r10d                      \n"
		"sbb %%r11, %%r11                       \n"
		"cmp %%r10d, %%edx                      \n"
		"adc $0, %%r11                          \n"
		"cmp %%ecx, %%edx                       \n"
		"adc %%rbx, %%rbx                       \n"
		"cmp %%r10d, %%ecx                      \n"
		"adc $0, %%r11                          \n"
		"movzb shuf4mu0+11(%%rbx,%%r11,8), %%r8 \n"
		"mov %%r10d,4(%0,%%r11,4)               \n"
		"movzb shuf4mu1+11(%%rbx,%%r11,8), %%r9 \n"
		"mov %%eax, (%0,%%r8,4)                 \n"
		"movzb shuf4mu2+11(%%rbx,%%r11,8), %%r8 \n"
		"mov %%edx, (%0,%%r9,4)                 \n"
		"mov %%ecx, (%0,%%r8,4)                 \n"
		: "+r"(buffer)
		:
		: "eax", "rbx", "ecx", "edx", "r8", "r9", "r10", "r11", "memory");
}

// Sort4ms - sort 4 int32_t values pointed to by buffer;
// reuses the write shuffle vectors from Sort4mu
void sort4_mimicry_ms(int32_t *buffer)
{
	asm volatile(
		"mov (%0), %%eax                        \n"
		"mov 4(%0), %%edx                       \n"
		"lea 0x80000000(%%eax), %%r12d          \n"
		"lea 0x80000000(%%edx), %%r13d          \n"
		"cmp %%r12d, %%r13d                     \n"
		"sbb %%rbx, %%rbx                       \n"
		"mov 8(%0), %%ecx                       \n"
		"mov 12(%0), %%r10d                     \n"
		"lea 0x80000000(%%ecx), %%r14d          \n"
		"lea 0x80000000(%%r10d), %%r9d          \n"
		"cmp %%r12d, %%r14d                     \n"
		"adc %%rbx, %%rbx                       \n"
		"cmp %%r12d, %%r9d                      \n"
		"sbb %%r11, %%r11                       \n"
		"cmp %%r9d, %%r13d                      \n"
		"adc $0, %%r11                          \n"
		"cmp %%r14d, %%r13d                     \n"
		"adc %%rbx, %%rbx                       \n"
		"cmp %%r9d, %%r14d                      \n"
		"adc $0, %%r11                          \n"
		"movzb shuf4mu0+11(%%rbx,%%r11,8), %%r8 \n"
		"mov %%r10d,4(%0,%%r11,4)               \n"
		"movzb shuf4mu1+11(%%rbx,%%r11,8), %%r9 \n"
		"mov %%eax, (%0,%%r8,4)                 \n"
		"movzb shuf4mu2+11(%%rbx,%%r11,8), %%r8 \n"
		"mov %%edx, (%0,%%r9,4)                 \n"
		"mov %%ecx, (%0,%%r8,4)                 \n"
		: "+r"(buffer)
		:
		: "eax", "rbx", "ecx", "edx", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "memory");
}

// Sort4mv - sort 4 int32_t values pointed to by buffer, using SIMD
// instructions

// Explanation: Sort4mv performs the six comparisons
//      v0 > v1, v1 > v2, v2 > v3, v3 > v0, v0 > v2, v1 > v3
// and packs the comparison result into a 6-bit vector, used for selecting
// a read shuffle vector. Note that not all indices are possible (filled with
// entries indicating invalid below), and that other combinations of six
// comparisons could be used to select the shuffle, with function finder
// having selected one that can be executed with a minimal number of shuffle
// operations in preparation for comparing values.
uint32_t shuffles[64 * 4] = {
	p0, p1, p2, p3,			// index 0
	invp, invp, invp, invp, // index 1
	invp, invp, invp, invp, // index 2
	invp, invp, invp, invp, // index 3
	p1, p2, p3, p0,			// index 4
	p1, p2, p3, p0,			// index 5
	invp, invp, invp, invp, // index 6
	invp, invp, invp, invp, // index 7
	invp, invp, invp, invp, // index 8
	p2, p3, p0, p1,			// index 9
	p0, p2, p3, p1,			// index 10
	p2, p3, p0, p1,			// index 11
	invp, invp, invp, invp, // index 12
	p2, p1, p3, p0,			// index 13
	invp, invp, invp, invp, // index 14
	p2, p3, p1, p0,			// index 15
	p0, p3, p1, p2,			// index 16
	invp, invp, invp, invp, // index 17
	p3, p0, p1, p2,			// index 18
	invp, invp, invp, invp, // index 19
	p1, p3, p0, p2,			// index 20
	p1, p3, p2, p0,			// index 21
	p3, p1, p0, p2,			// index 22
	p3, p1, p2, p0,			// index 23
	invp, invp, invp, invp, // index 24
	invp, invp, invp, invp, // index 25
	p3, p0, p2, p1,			// index 26
	p3, p2, p0, p1,			// index 27
	invp, invp, invp, invp, // index 28
	invp, invp, invp, invp, // index 29
	invp, invp, invp, invp, // index 30
	p3, p2, p1, p0,			// index 31
	p0, p1, p2, p3,			// index 32
	invp, invp, invp, invp, // index 33
	invp, invp, invp, invp, // index 34
	invp, invp, invp, invp, // index 35
	p1, p0, p2, p3,			// index 36
	p1, p2, p0, p3,			// index 37
	invp, invp, invp, invp, // index 38
	invp, invp, invp, invp, // index 39
	p0, p2, p1, p3,			// index 40
	p2, p0, p1, p3,			// index 41
	p0, p2, p3, p1,			// index 42
	p2, p0, p3, p1,			// index 43
	invp, invp, invp, invp, // index 44
	p2, p1, p0, p3,			// index 45
	invp, invp, invp, invp, // index 46
	invp, invp, invp, invp, // index 47
	p0, p1, p3, p2,			// index 48
	invp, invp, invp, invp, // index 49
	p0, p3, p1, p2,			// index 50
	invp, invp, invp, invp, // index 51
	p1, p0, p3, p2,			// index 52
	invp, invp, invp, invp, // index 53
	invp, invp, invp, invp, // index 54
	invp, invp, invp, invp, // index 55
	invp, invp, invp, invp, // index 56
	invp, invp, invp, invp, // index 57
	p0, p3, p2, p1,			// index 58
	invp, invp, invp, invp, // index 59
	invp, invp, invp, invp, // index 60
	invp, invp, invp, invp, // index 61
	invp, invp, invp, invp, // index 62
	invp, invp, invp, invp	// index 63
};

void sort4_mimicry_mv(int32_t *buffer)
{
	asm volatile(
		"vmovdqu (%0), %%xmm1            \n"
		"vpshufd $0x39, %%xmm1, %%xmm3   \n" // shuffle 0b00111001 -> 1, 2, 3, 0
		"vpshufd $0xee, %%xmm1, %%xmm4   \n" // shuffle 0b11101110 -> 2, 3, 2, 3
		"vpcmpgtd %%xmm3, %%xmm1, %%xmm3 \n"
		"vpcmpgtd %%xmm4, %%xmm1, %%xmm4 \n"
		"movmskps %%xmm3, %%eax          \n"
		"movmskps %%xmm4, %%ecx          \n"
		"lea 0(%%ecx,%%eax,4), %%eax     \n"
		"shl $4, %%eax                   \n"
		"movdqa shuffles(%%eax), %%xmm2  \n"
		"vpshufb %%xmm2, %%xmm1, %%xmm1  \n"
		"vmovdqu %%xmm1, (%0)            \n"
		: "+r"(buffer)
		:
		: "xmm1", "xmm2", "xmm3", "xmm4", "eax", "ecx");
}
