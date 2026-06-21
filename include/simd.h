#pragma once
#include <cstdint>
#include <cstddef>

#if defined(__x86_64__) || defined(__amd64__)
    #include <immintrin.h>
#elif defined(__aarch64__) || defined(__arm__)
    #include <arm_neon.h>
#endif


namespace simd {
// Linux x85 gcc/clang
#if defined(__AVX512BW__) && defined(__AVX512F__) // 64B - incl byte/word pairs
    using simd_mask = std::uint64_t;
    inline constexpr std::size_t kStride = sizeof(__m512i);
#elif defined(__AVX2__) // 32B
    using simd_mask = std::uint32_t;
    inline constexpr std::size_t kStride = sizeof(__m256i);
#elif defined(__SSE4_2__) // 16B - incl pcmpistri
    using simd_mask = std::uint16_t;
    inline constexpr std::size_t kStride = sizeof(__m128i);
#elif defined(__SSE2__) // 16B
    using simd_mask = std::uint16_t;
    inline constexpr std::size_t kStride = sizeof(__m128i);
#elif defined(__ARM_NEON__) || defined (__ARM_NEON) || \
        defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC) // ARM 64
    using simd_mask = std::uint64_t;
    inline constexpr std::size_t kStride = sizeof(uint8x16_t);
#else
    using simd_mask = std::unint8_t;
    inline constexpr std::size_t kStride = 1;
#endif
}
