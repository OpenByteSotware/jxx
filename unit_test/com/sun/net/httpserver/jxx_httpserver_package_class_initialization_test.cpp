#include <gtest/gtest.h>

#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Authenticator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.ClassInitialization.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Filter.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ClassInitialization.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ReadOnlyList.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ReadOnlyViews.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace
{
    template <typename T>
    void expectClassMetadata()
    {
        const auto type = T::Class();
        ASSERT_NE(type, nullptr);
        ASSERT_NE(type->getName(), nullptr);
        EXPECT_FALSE(type->getName()->utf8().empty());
    }

    TEST(HttpServerPackageClassInitialization, RegistersNestedPublicClasses)
    {
        using namespace ::jxx::com::sun::net::httpserver;
        initializeClasses();
        expectClassMetadata<Authenticator::Success>();
        expectClassMetadata<Authenticator::Failure>();
        expectClassMetadata<Authenticator::Retry>();
        expectClassMetadata<Filter::Chain>();
    }

    TEST(HttpServerInternalClassInitialization, RegistersReadOnlyViewSpecializations)
    {
        using namespace ::jxx::com::sun::net::httpserver::internal;
        using Object = ::jxx::lang::Object;
        initializeClasses();
        expectClassMetadata<ReadOnlyListIterator<Object>>();
        expectClassMetadata<ReadOnlyList<Object>>();
        expectClassMetadata<ReadOnlyIterator<Object>>();
        expectClassMetadata<ReadOnlyMapEntry<Object, Object>>();
        expectClassMetadata<ReadOnlyEntryIterator<Object, Object>>();
        expectClassMetadata<ReadOnlyCollection<Object>>();
        expectClassMetadata<ReadOnlySet<Object>>();
        expectClassMetadata<ReadOnlyEntrySet<Object, Object>>();
    }
}
