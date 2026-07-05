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
}

void Parsing::SlowPass::process(const LineDescriptor& line, Findings& out) const {

}

constexpr void Parsing::SlowPass::rebase(Finding& finding, const char* base_pos) noexcept {

}
