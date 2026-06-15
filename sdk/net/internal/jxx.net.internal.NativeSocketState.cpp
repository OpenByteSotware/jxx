#include "net/internal/jxx.net.internal.NativeSocketState.h"

#include <stdexcept>

#if defined(_WIN32)
    #include <winsock2.h>
#else
    #include <sys/ioctl.h>
    #include <sys/socket.h>
#endif

namespace
{
    [[noreturn]] void throwIOE_(const char* msg)
    {
        throw std::runtime_error(msg);
    }
}

namespace jxx::net::internal
{
    NativeSocketInputStream::NativeSocketInputStream(std::shared_ptr<NativeSocketState> state)
        : state_(std::move(state))
    {
    }

    jxx::lang::jint NativeSocketInputStream::read()
    {
        auto arr = std::make_shared<jxx::JxxArray<jxx::lang::jbyte, 1U>>(1);
        const auto n = read(arr, 0, 1);
        return n <= 0 ? -1 : static_cast<jxx::lang::jint>((*arr)[0] & 0xFF);
    }

    jxx::lang::jint NativeSocketInputStream::read(const jxx::lang::ByteArray b,
                                                  jxx::lang::jint off,
                                                  jxx::lang::jint len)
    {
        if (!state_ || !b)
            throwIOE_("null state/buffer");
        if (len == 0)
            return 0;

        std::lock_guard<std::mutex> lock(state_->m);
        if (state_->closed || state_->inputShutdown || state_->socket == kInvalidSocket)
            return -1;

        const auto rc = ::recv(state_->socket,
                               reinterpret_cast<char*>(&(*b)[off]),
                               static_cast<int>(len),
                               0);
        if (rc == 0)
            return -1;
        if (rc < 0)
            throwIOE_("socket recv failed");
        return static_cast<jxx::lang::jint>(rc);
    }

    jxx::lang::jint NativeSocketInputStream::available()
    {
        if (!state_)
            return 0;
        std::lock_guard<std::mutex> lock(state_->m);
        if (state_->closed || state_->socket == kInvalidSocket)
            return 0;
    #if defined(_WIN32)
        u_long bytes = 0;
        if (::ioctlsocket(state_->socket, FIONREAD, &bytes) != 0)
            return 0;
        return static_cast<jxx::lang::jint>(bytes);
    #else
        int bytes = 0;
        if (::ioctl(state_->socket, FIONREAD, &bytes) != 0)
            return 0;
        return static_cast<jxx::lang::jint>(bytes);
    #endif
    }

    void NativeSocketInputStream::close()
    {
        if (!state_)
            return;
        std::lock_guard<std::mutex> lock(state_->m);
        state_->inputShutdown = true;
    }

    NativeSocketOutputStream::NativeSocketOutputStream(std::shared_ptr<NativeSocketState> state)
        : state_(std::move(state))
    {
    }

    void NativeSocketOutputStream::write(jxx::lang::jint b)
    {
        auto arr = std::make_shared<jxx::JxxArray<jxx::lang::jbyte, 1U>>(1);
        (*arr)[0] = static_cast<jxx::lang::jbyte>(b);
        write(arr, 0, 1);
    }

    void NativeSocketOutputStream::write(const jxx::lang::ByteArray b,
                                         jxx::lang::jint off,
                                         jxx::lang::jint len)
    {
        if (!state_ || !b)
            throwIOE_("null state/buffer");
        if (len == 0)
            return;

        std::lock_guard<std::mutex> lock(state_->m);
        if (state_->closed || state_->outputShutdown || state_->socket == kInvalidSocket)
            throwIOE_("socket closed for output");

        const auto rc = ::send(state_->socket,
                               reinterpret_cast<const char*>(&(*b)[off]),
                               static_cast<int>(len),
                               0);
        if (rc < 0 || rc != len)
            throwIOE_("socket send failed");
    }

    void NativeSocketOutputStream::flush()
    {
    }

    void NativeSocketOutputStream::close()
    {
        if (!state_)
            return;
        std::lock_guard<std::mutex> lock(state_->m);
        state_->outputShutdown = true;
    }
}
