
#pragma once
#include <memory>
#include <vector>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.InputStream.h"

namespace jxx { namespace io {

class SequenceInputStream : public InputStream {
    std::vector<std::shared_ptr<InputStream>> seq;
    std::size_t idx = 0;
public:
    SequenceInputStream(std::shared_ptr<InputStream> s1, std::shared_ptr<InputStream> s2);
    explicit SequenceInputStream(std::vector<std::shared_ptr<InputStream>> streams);

    int read() override;
    int read(ByteArray& b, int off, int len) override;
    int available() override;
    void close() override;
};

}}
