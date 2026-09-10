#pragma once

#include <cstddef>
#include <vector>

#include "io/jxx.io.InputStream.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.Enumeration.h"

namespace jxx::io {

class SequenceInputStream
    : public ::jxx::lang::ClassBase<SequenceInputStream, InputStream> {
public:
    using JxxSuper = InputStream;
    using Super = ::jxx::lang::ClassBase<SequenceInputStream, JxxSuper>;
    using JxxSuper::read;

    explicit SequenceInputStream(
        const ::jxx::Ptr<::jxx::util::Enumeration<InputStream>>& streams);
    SequenceInputStream(const ::jxx::Ptr<InputStream>& first,
                        const ::jxx::Ptr<InputStream>& second);
    ~SequenceInputStream() override;

    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer,
                           ::jxx::lang::jint offset,
                           ::jxx::lang::jint length) override;
    ::jxx::lang::jint available() override;
    void close() override;

private:
    void advance_();
    std::vector<::jxx::Ptr<InputStream>> streams_;
    std::size_t index_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

} // namespace jxx::io
