#include <gtest/gtest.h>
#include <type_traits>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslEngine.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLEngineResult.h"
#include "nio/jxx.nio.ByteBuffer.h"

namespace {

TEST(
    EngineUnwrapAccountingParityTest,
    UnwrapSurfaceReturnsEngineResult)
{
    using Engine =
        ::jxx::ext::net::ssl::internal::
            OpenSslEngine;

    using Method =
        ::jxx::Ptr<
            ::jxx::ext::net::ssl::
                SSLEngineResult> (Engine::*)(
                    const ::jxx::Ptr<
                        ::jxx::nio::ByteBuffer>&,
                    const ::jxx::Ptr<
                        ::jxx::nio::ByteBuffer>&);

    EXPECT_TRUE((std::is_same_v<
        decltype(static_cast<Method>(
            &Engine::unwrap)),
        Method>));
}

} // namespace
