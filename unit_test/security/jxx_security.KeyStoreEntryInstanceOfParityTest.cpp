#include <gtest/gtest.h>
#include <type_traits>

#include "lang/jxx.lang.Class.h"
#include "security/jxx.security.KeyStore.h"

namespace {

TEST(
    KeyStoreEntryInstanceOfParityTest,
    SurfaceAcceptsClassAny)
{
    using Method = ::jxx::lang::jbool (
        ::jxx::security::KeyStore::*)(
            const ::jxx::Ptr<::jxx::lang::String>&,
            const ::jxx::Ptr<::jxx::lang::ClassAny>&) const;

    EXPECT_TRUE((std::is_same_v<
        decltype(static_cast<Method>(
            &::jxx::security::KeyStore::entryInstanceOf)),
        Method>));
}

} // namespace
