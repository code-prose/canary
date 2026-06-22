#pragma once
#include <cstdint>
#include "simd.h"


namespace Parsing {
    enum class Candidate : std::uint8_t {
        None = 0,
        Digit = 1 << 0,
        At = 1 << 1,

    };

    inline constexpr Candidate operator|(Candidate A, Candidate B) noexcept {
        return static_cast<Candidate>(static_cast<std::uint8_t>(A) | static_cast<std::uint8_t>(B));
    }
    inline constexpr Candidate& operator|=(Candidate& A, Candidate B) noexcept {
        A = static_cast<Candidate>(static_cast<std::uint8_t>(A) | static_cast<std::uint8_t>(B));
        return A;
    }
    inline constexpr Candidate operator&(Candidate A, Candidate B) noexcept {
        return static_cast<Candidate>(static_cast<std::uint8_t>(A) & static_cast<std::uint8_t>(B));
    }
    inline constexpr bool has(Candidate c, Candidate bit) noexcept {
        return (static_cast<std::uint8_t>(c) & static_cast<std::uint8_t>(bit)) != 0;
    }

    struct LineDescriptor {
        const char* start;
        std::uint32_t len; // does not include \n, \n\n, len == 0
        std::uint8_t flags;
    };

    class FastRejectBase {
    };

    class SlowPass {

    };

} // namespace Parsing
