
#pragma once
#include <memory>
#include <vector>
#include "jxx/io/jxx.io.InputStream.h"

namespace jxx { namespace io {

class SequenceInputStream : public InputStream {
    std::vector<std::shared_ptr<InputStream>> seq;
    std::size_t idx = 0;
public:
    SequenceInputStream(std::shared_ptr<InputStream> s1, std::shared_ptr<InputStream> s2);
    explicit SequenceInputStream(std::vector<std::shared_ptr<InputStream>> streams);

    int read() override;
    jxx::lang::jint read(ByteArray& b, jxx::lang::jint off, jxx::lang::jint len) override;
    int available() override;
    void close() override;
};

}}
