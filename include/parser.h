#pragma once
#include <cstdint>
#include <vector>
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

    enum class Kind : std::uint8_t {
        Ssn,
        CardNumber,
        Email,
        UID,
        FalsePos
    };

    struct Finding {
        Kind kind;
        std::uint64_t pos;
        std::uint32_t len;
    };

    using Findings = std::vector<Finding>;

    struct Report {
        Findings all;
    };

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


    class SlowPass {
        public:
            void process(const LineDescriptor& line, Findings& out) const;
            static constexpr void rebase(Finding& finding, const char* base_pos) noexcept;
        private:
    };

    class Parser {
        public:
            Report run(const std::string& path);
        private:
            LineRing buf_{};
            std::size_t mmap_len_;

            // Will be used when I swap from spsc to fan out
            // Findings merge(std::vector<Findings>& all);
    };

} // namespace Parsing
