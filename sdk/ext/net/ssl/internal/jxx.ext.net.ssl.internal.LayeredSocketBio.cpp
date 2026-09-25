#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.LayeredSocketBio.h"

#include <algorithm>
#include <cerrno>
#include <climits>
#include <utility>

#if defined(_WIN32)
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

namespace jxx::ext::net::ssl::internal {
namespace {

int layeredCreate(BIO* bio) {
    BIO_set_init(bio, 1);
    BIO_set_data(bio, nullptr);
    BIO_set_shutdown(bio, 0);
    return 1;
}

int layeredDestroy(BIO* bio) {
    if (bio == nullptr) return 0;
    delete static_cast<LayeredSocketBioState*>(BIO_get_data(bio));
    BIO_set_data(bio, nullptr);
    BIO_set_init(bio, 0);
    return 1;
}

int layeredRead(BIO* bio, char* output, int length) {
    if (output == nullptr || length <= 0) return 0;
    auto* state = static_cast<LayeredSocketBioState*>(BIO_get_data(bio));
    if (state == nullptr) return -1;
    BIO_clear_retry_flags(bio);
    if (state->consumedOffset < state->consumed.size()) {
        const auto remaining = state->consumed.size() - state->consumedOffset;
        const auto count = std::min<std::size_t>(remaining, static_cast<std::size_t>(length));
        std::copy_n(state->consumed.data() + state->consumedOffset, count,
                    reinterpret_cast<unsigned char*>(output));
        state->consumedOffset += count;
        return static_cast<int>(count);
    }
#if defined(_WIN32)
    const int result = ::recv(state->socket, output, length, 0);
    if (result == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK) BIO_set_retry_read(bio);
#else
    const int result = static_cast<int>(::recv(state->socket, output, static_cast<std::size_t>(length), 0));
    if (result < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) BIO_set_retry_read(bio);
#endif
    return result;
}

int layeredWrite(BIO* bio, const char* input, int length) {
    if (input == nullptr || length <= 0) return 0;
    auto* state = static_cast<LayeredSocketBioState*>(BIO_get_data(bio));
    if (state == nullptr) return -1;
    BIO_clear_retry_flags(bio);
#if defined(_WIN32)
    const int result = ::send(state->socket, input, length, 0);
    if (result == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK) BIO_set_retry_write(bio);
#else
    const int result = static_cast<int>(::send(state->socket, input, static_cast<std::size_t>(length), 0));
    if (result < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) BIO_set_retry_write(bio);
#endif
    return result;
}

long layeredControl(BIO*, int command, long, void*) {
    if (command == BIO_CTRL_FLUSH) return 1;
    return 0;
}

BIO_METHOD* layeredMethod() {
    static BIO_METHOD* method = [] {
        BIO_METHOD* value = BIO_meth_new(BIO_TYPE_SOURCE_SINK, "jxx layered socket");
        BIO_meth_set_create(value, layeredCreate);
        BIO_meth_set_destroy(value, layeredDestroy);
        BIO_meth_set_read(value, layeredRead);
        BIO_meth_set_write(value, layeredWrite);
        BIO_meth_set_ctrl(value, layeredControl);
        return value;
    }();
    return method;
}

} // namespace

LayeredSocketBioState::LayeredSocketBioState(
    ::jxx::net::internal::NativeSocket socketValue,
    std::vector<unsigned char> consumedValue)
    : socket(socketValue)
    , consumed(std::move(consumedValue)) {
}

BIO* createLayeredSocketBio(
    ::jxx::net::internal::NativeSocket socket,
    std::vector<unsigned char> consumed) {
    BIO* bio = BIO_new(layeredMethod());
    if (bio == nullptr) return nullptr;
    BIO_set_data(bio, new LayeredSocketBioState(socket, std::move(consumed)));
    BIO_set_init(bio, 1);
    return bio;
}

} // namespace jxx::ext::net::ssl::internal
