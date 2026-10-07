/* The real MS x64 ABI, independently compiled, not another Caustic call. */
#include <stdint.h>

typedef uint64_t (*Wide)(uint64_t, uint64_t, uint64_t, uint64_t,
                         uint64_t, uint64_t, uint64_t, uint64_t,
                         uint64_t, uint64_t, uint64_t, uint64_t,
                         uint64_t, uint64_t, uint64_t, uint64_t);
typedef uint64_t (*Mixed)(uint64_t, double, uint64_t, double, uint64_t, double);

__declspec(dllexport) uint64_t NativeWide(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3,
    uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8, uint64_t a9,
    uint64_t a10, uint64_t a11, uint64_t a12, uint64_t a13, uint64_t a14, uint64_t a15) {
    const uint64_t args[] = {a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15};
    for (int i = 0; i < 16; ++i)
        if (args[i] != UINT64_C(0xabcdef1200000000) + i) return 0;
    return UINT64_C(0x123456789abcdef0);
}

__declspec(dllexport) uint64_t NativeMixed(uint64_t a, double b, uint64_t c, double d,
                                          uint64_t e, double f) {
    return a == 31 && b == 1.25 && c == 33 && d == -2.5 && e == 35 && f == 3.75;
}

__declspec(dllexport) uint64_t ExerciseWide(Wide cb) {
    return cb(UINT64_C(0xabcdef1200000000),UINT64_C(0xabcdef1200000001),
              UINT64_C(0xabcdef1200000002),UINT64_C(0xabcdef1200000003),
              UINT64_C(0xabcdef1200000004),UINT64_C(0xabcdef1200000005),
              UINT64_C(0xabcdef1200000006),UINT64_C(0xabcdef1200000007),
              UINT64_C(0xabcdef1200000008),UINT64_C(0xabcdef1200000009),
              UINT64_C(0xabcdef120000000a),UINT64_C(0xabcdef120000000b),
              UINT64_C(0xabcdef120000000c),UINT64_C(0xabcdef120000000d),
              UINT64_C(0xabcdef120000000e),UINT64_C(0xabcdef120000000f));
}

__declspec(dllexport) uint64_t ExerciseMixed(Mixed cb) {
    return cb(31, 1.25, 33, -2.5, 35, 3.75);
}

/* The callback deliberately clobbers all SysV-volatile XMM registers. These
   full-width sentinels must survive the boundary, including their upper halves. */
__declspec(dllexport) uint64_t ExercisePreserved(Wide cb) {
    const uint64_t pattern[2] = {UINT64_C(0xabcdef1234567890), UINT64_C(0x123456789abcdef0)};
    uint64_t got[10][2];
    __asm__ volatile (
        "movdqu %0, %%xmm6\n\tmovdqu %0, %%xmm7\n\tmovdqu %0, %%xmm8\n\t"
        "movdqu %0, %%xmm9\n\tmovdqu %0, %%xmm10\n\tmovdqu %0, %%xmm11\n\t"
        "movdqu %0, %%xmm12\n\tmovdqu %0, %%xmm13\n\tmovdqu %0, %%xmm14\n\tmovdqu %0, %%xmm15"
        : : "m"(pattern) : "xmm6","xmm7","xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15");
    uint64_t result = ExerciseWide(cb);
    __asm__ volatile (
        "movdqu %%xmm6, %0\n\tmovdqu %%xmm7, %1\n\tmovdqu %%xmm8, %2\n\t"
        "movdqu %%xmm9, %3\n\tmovdqu %%xmm10, %4\n\tmovdqu %%xmm11, %5\n\t"
        "movdqu %%xmm12, %6\n\tmovdqu %%xmm13, %7\n\tmovdqu %%xmm14, %8\n\tmovdqu %%xmm15, %9"
        : "=m"(got[0]),"=m"(got[1]),"=m"(got[2]),"=m"(got[3]),"=m"(got[4]),
          "=m"(got[5]),"=m"(got[6]),"=m"(got[7]),"=m"(got[8]),"=m"(got[9]));
    if (result != UINT64_C(0x123456789abcdef0)) return 0;
    for (int i = 0; i < 10; ++i)
        if (got[i][0] != pattern[0] || got[i][1] != pattern[1]) return 0;
    return 1;
}

__declspec(dllexport) void *WideAddress(void) { return (void *)NativeWide; }
__declspec(dllexport) void *MixedAddress(void) { return (void *)NativeMixed; }
