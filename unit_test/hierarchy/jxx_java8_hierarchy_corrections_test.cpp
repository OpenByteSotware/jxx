#include <gtest/gtest.h>
#include <type_traits>

#include "ext/security/cert/jxx.ext.security.cert.Certificate.h"
#include "ext/security/cert/jxx.ext.security.cert.X509Certificate.h"
#include "io/jxx.io.DataOutput.h"
#include "io/jxx.io.Flushable.h"
#include "io/jxx.io.ObjectOutput.h"
#include "lang/jxx.lang.AutoCloseable.h"
#include "lang/jxx.lang.Object.h"
#include "util/concurrent/jxx.util.concurrent.CancellableTask.h"
#include "util/concurrent/jxx.util.concurrent.Future.h"

namespace {

TEST(JxxJava8HierarchyCorrectionsTest, LegacyX509CertificateExtendsCertificate)
{
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::ext::security::cert::Certificate,
        ::jxx::ext::security::cert::X509Certificate>));
}

TEST(JxxJava8HierarchyCorrectionsTest, ObjectOutputHasExactParentInterfaces)
{
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::io::DataOutput, ::jxx::io::ObjectOutput>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::AutoCloseable, ::jxx::io::ObjectOutput>));
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::io::Flushable, ::jxx::io::ObjectOutput>));
}

TEST(JxxJava8HierarchyCorrectionsTest, FutureHasNoInternalParentInterface)
{
    using FutureObject =
        ::jxx::util::concurrent::Future<::jxx::lang::Object>;
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::util::concurrent::internal::CancellableTask,
        FutureObject>));
}

} // namespace
