#pragma once

#include <memory>

#include "io/jxx.io.InputStream.h"
#include "lang/jxx_types.h"

namespace jxx::io::internal { struct PipeState; }
namespace jxx::io { class PipedOutputStream;

class PipedInputStream
    : public ::jxx::lang::ClassBase<PipedInputStream, InputStream> {
public:
    using JxxSuper = InputStream;
    using Super = ::jxx::lang::ClassBase<PipedInputStream, JxxSuper>;

    static constexpr ::jxx::lang::jint PIPE_SIZE = 1024;

    PipedInputStream();
    explicit PipedInputStream(::jxx::lang::jint pipeSize);
    explicit PipedInputStream(const ::jxx::Ptr<PipedOutputStream>& source);
    PipedInputStream(const ::jxx::Ptr<PipedOutputStream>& source,
                     ::jxx::lang::jint pipeSize);
    ~PipedInputStream() override;

    void connect(const ::jxx::Ptr<PipedOutputStream>& source);
    ::jxx::lang::jint read() override;
    ::jxx::lang::jint read(const ::jxx::lang::ByteArray& buffer,
                           ::jxx::lang::jint offset,
                           ::jxx::lang::jint length) override;
    ::jxx::lang::jint available() override;
    void close() override;

private:
    friend class PipedOutputStream;
    static ::jxx::lang::jint validatePipeSize_(::jxx::lang::jint pipeSize);
    void attach_(const std::shared_ptr<internal::PipeState>& state);
    std::shared_ptr<internal::PipeState> state_;
};

} // namespace jxx::io
