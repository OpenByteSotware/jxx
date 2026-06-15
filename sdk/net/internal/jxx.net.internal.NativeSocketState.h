#pragma once

#include <memory>
#include <mutex>

#include "jxx_types.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"

namespace jxx::net::internal
{
    struct NativeSocketState
    {
        NativeSocket socket = kInvalidSocket;
        jxx::lang::jbool closed = false;
        jxx::lang::jbool inputShutdown = false;
        jxx::lang::jbool outputShutdown = false;
        std::mutex m;
    };

    class NativeSocketInputStream final : public jxx::io::InputStream
    {
    public:
        explicit NativeSocketInputStream(std::shared_ptr<NativeSocketState> state);
        ~NativeSocketInputStream() override = default;

    public:
        jxx::lang::jint read() override;
        jxx::lang::jint read(const jxx::lang::ByteArray b,
                             jxx::lang::jint off,
                             jxx::lang::jint len) override;
        jxx::lang::jint available() override;
        void close() override;

    private:
        std::shared_ptr<NativeSocketState> state_;
    };

    class NativeSocketOutputStream final : public jxx::io::OutputStream
    {
    public:
        explicit NativeSocketOutputStream(std::shared_ptr<NativeSocketState> state);
        ~NativeSocketOutputStream() override = default;

    public:
        void write(jxx::lang::jint b) override;
        void write(const jxx::lang::ByteArray b,
                   jxx::lang::jint off,
                   jxx::lang::jint len) override;
        void flush() override;
        void close() override;

    private:
        std::shared_ptr<NativeSocketState> state_;
    };
}
