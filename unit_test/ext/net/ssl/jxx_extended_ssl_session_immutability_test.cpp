#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SNIHostName.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.UnmodifiableArrayList.h"

namespace {
::jxx::Ptr<::jxx::lang::String> text(const char* value) {
    return ::jxx::NEW<::jxx::lang::String>(value);
}
}

TEST(ExtendedSSLSessionImmutabilityTest, EmptyRequestedNamesViewIsImmutable) {
    using Name = ::jxx::ext::net::ssl::SNIServerName;
    auto names = ::jxx::NEW<::jxx::util::UnmodifiableArrayList<Name>>();
    EXPECT_EQ(0, names->size());
    EXPECT_THROW(names->clear(), ::jxx::lang::UnsupportedOperationException);
    EXPECT_THROW(
        names->add(::jxx::NEW<::jxx::ext::net::ssl::SNIHostName>(
            text("example.test"))),
        ::jxx::lang::UnsupportedOperationException);
}

TEST(ExtendedSSLSessionImmutabilityTest, RequestedNamesAreCopiedAndImmutable) {
    using Name = ::jxx::ext::net::ssl::SNIServerName;
    auto source = ::jxx::NEW<::jxx::util::ArrayList<Name>>();
    source->add(::jxx::NEW<::jxx::ext::net::ssl::SNIHostName>(
        text("example.test")));
    auto names = ::jxx::NEW<::jxx::util::UnmodifiableArrayList<Name>>(
        ::jxx::CAST<::jxx::util::List<Name>>(source));
    source->clear();
    EXPECT_EQ(1, names->size());
    EXPECT_THROW(names->remove(0),
        ::jxx::lang::UnsupportedOperationException);
    EXPECT_THROW(names->set(0, nullptr),
        ::jxx::lang::UnsupportedOperationException);
}
