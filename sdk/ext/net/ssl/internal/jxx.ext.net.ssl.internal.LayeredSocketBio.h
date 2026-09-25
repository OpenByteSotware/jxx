#pragma once

#include <cstddef>
#include <vector>

#include <openssl/bio.h>

#include "net/internal/jxx.net.internal.NetPlatform.h"

namespace jxx::ext::net::ssl::internal {

class LayeredSocketBioState final {
public:
    LayeredSocketBioState(
        ::jxx::net::internal::NativeSocket socket,
        std::vector<unsigned char> consumed);

    ::jxx::net::internal::NativeSocket socket;
    std::vector<unsigned char> consumed;
    std::size_t consumedOffset = 0;
};

BIO* createLayeredSocketBio(
    ::jxx::net::internal::NativeSocket socket,
    std::vector<unsigned char> consumed);

} // namespace jxx::ext::net::ssl::internal
