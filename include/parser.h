#pragma once
#include <cstdint>
#include "ring_buffer.h"
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

    inline constexpr std::size_t LineRingCap = 1 << 14;
    using LineRing = SPSC::RingBuffer<LineDescriptor, LineRingCap>;

    class FastReject {
        public:
            void scan(const char* begin, const char* end, LineRing& out) noexcept;
    };

    // placeholder
    using Findings = std::size_t;

    class SlowPass {
        public:
            Findings process(const LineDescriptor& line) const;
        private:
    };

    class Parser {
        public:
        private:
            const char* mmap_;
            LineRing buf_{};
    };

} // namespace Parsing
