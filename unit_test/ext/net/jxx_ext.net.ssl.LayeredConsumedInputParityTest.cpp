#include <gtest/gtest.h>

#include <vector>

#include <openssl/bio.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.LayeredSocketBio.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"

namespace {

TEST(LayeredConsumedInputParityTest, ReplaysConsumedBytesBeforeSocketRead) {
    const std::vector<unsigned char> prefix = {
        0x16,
        0x03,
        0x03,
        0x00,
        0x02,
        0x01,
        0x00
    };

    BIO* bio =
        ::jxx::ext::net::ssl::internal::
            createLayeredSocketBio(
                ::jxx::net::internal::
                    kInvalidSocket,
                prefix);

    ASSERT_NE(bio, nullptr);

    unsigned char output[7] = {};
    const int count = BIO_read(
        bio,
        output,
        static_cast<int>(sizeof(output)));

    ASSERT_EQ(count, 7);

    for (int index = 0; index < count; ++index) {
        EXPECT_EQ(
            output[index],
            prefix[static_cast<std::size_t>(index)]);
    }

    BIO_free(bio);
}

} // namespace
