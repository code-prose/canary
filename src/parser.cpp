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

    auto consumer = []{
        SlowPass sp{};
    };
}
