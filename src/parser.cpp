#include <string>
#include <thread>

#include "parser.h"
#include "mapping.h"

// need different static inits for simd::mask
//
//
// need to check .done() in consumer
//

// Parser
Parsing::Report Parsing::Parser::run(const std::string& path) {
    Mapping m{path};
    const char* mmap_;
    Report report{};

    auto producer = [&]{
        FastReject frj{};
        frj.scan(m.data(), m.data() + m.length(), buf_);
    };

    auto consumer = [&]{
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
    };

    // some alerting code
}
