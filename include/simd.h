#pragma once

#if defined(__x86_64__) || defined(__amd64__)
    #include <immintrin.h>
#endif

#if defined(__aarch64__) || defined(__arm__)
    #include <arm_neon.h>
#endif

// ARM
#if defined(__ARM_NEON__) || defined (__ARM_NEON)
#endif

#if defined(__arch64__) || defined(_M_ARM_64) || defined(_M_ARM64EC)
#endif

#if defined(__ARM_FETATURE_SVE)

#endif

#if defined(__ARM_FETATURE_SVE2)

#endif


// Linux x85 gcc/clang
#if defined(__SSE2__) // 16B

#endif

#if defined(__SSE4_2__) // 16B - incl pcmpistri

#endif

#if defined(__AVX2__) // 32B
#endif

#if defined(__AVX512F__) // 64B
#endif


#if defined(__AVX512BW__) // 64B - incl byte/word pairs
#endif

#if defined(__AVX512VBMI__) // 64B - vpermb/multishift
#endif
