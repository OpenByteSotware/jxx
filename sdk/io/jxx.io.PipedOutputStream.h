#pragma once

#include <memory>

#include "io/jxx.io.OutputStream.h"
#include "lang/jxx_types.h"

namespace jxx::io::internal { struct PipeState; }
namespace jxx::io { class PipedInputStream;

class PipedOutputStream
    : public ::jxx::lang::ClassBase<PipedOutputStream, OutputStream> {
public:
    using JxxSuper = OutputStream;
    using Super = ::jxx::lang::ClassBase<PipedOutputStream, JxxSuper>;

    PipedOutputStream();
    explicit PipedOutputStream(const ::jxx::Ptr<PipedInputStream>& sink);
    ~PipedOutputStream() override;

    void connect(const ::jxx::Ptr<PipedInputStream>& sink);
    void write(::jxx::lang::jint value) override;
    void write(const ::jxx::lang::ByteArray& buffer,
               ::jxx::lang::jint offset,
               ::jxx::lang::jint length) override;
    void flush() override;
    void close() override;

private:
    std::shared_ptr<internal::PipeState> state_;
};

} // namespace jxx::io
