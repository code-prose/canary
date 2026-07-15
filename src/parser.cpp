#include <arm_neon.h>
#include <string>
#include <thread>
#include "parser.h"
#include "mapping.h"

// need different static inits for simd::mask

// Parser
Parsing::Report Parsing::Parser::run(const std::string& path) {
    Mapping m{path};
    const char* mmap_;
    Report report{};

    auto prod = [&m, this]{
        FastReject frj{};
        frj.scan(m.data(), m.data() + m.length(), buf_);
    };

    // this feels so ugly
    auto cons = [&m, &report, this]{
        SlowPass sp{};
        bool val_returned = false;
        Findings findings{};
        while(!buf_.closed()) {
            LineDescriptor ld{};
            val_returned = buf_.pop(ld);
            if (val_returned) [[likely]] {
                sp.process(ld, findings); 
            }
        }

        for (auto& finding : findings) { sp.rebase(finding, m.data()); }
        report.all = findings;
    };

    std::thread producer{prod};
    std::thread consumer{cons};

    producer.detach();
    consumer.join();

    return report;
}


void Parsing::FastReject::scan(const char* begin, const char* end, LineRing& out) noexcept {
    // wrote overload for universal ref so I can std::move()
    // what the fuck does this refer to?
    // ringbuf?
    // yes
    const uint8x16_t zero_mask{vdupq_n_u8('0')};
    const uint8x16_t nine_mask{vdupq_n_u8('9')}; // why doesn't this work?
    const uint8x16_t colon_mask{vdupq_n_u8(':')};
    while (begin + simd::kStride <= end) {
        Candidate cand = Candidate::None;
        auto chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(begin));
        auto mask = vandq_u8(
                vcgeq_u8(chunk, zero_mask),
                vcgeq_u8(nine_mask, chunk));
        auto hits = vshrq_n_u8(mask, 7);
        auto count = vaddvq_u8(hits);

        auto colons = vceqq_u8(chunk, colon_mask);
        auto colon_hit = vmaxvq_u8(colons) != 0;

        auto ssnish = !colon_hit && count > 5;
        cand = ssnish ? cand | Candidate::Digit : cand; // collapsed branch prediction, csel now

        // also a branch prediction issue, this is explicitly the hot path so I don't want to be guessing at branches
        // this is unlikely? So it's okay? Depends on tuning probably
        if (cand != Candidate::None) [[unlikely]] {
            out.push(LineDescriptor{.start = begin, .len = simd::kStride, .flags = cand});
        }

        begin += simd::kStride;
    }
}

void Parsing::SlowPass::process(const LineDescriptor& line, Findings& out) const {

}

constexpr void Parsing::SlowPass::rebase(Finding& finding, const char* base_pos) noexcept {

}
