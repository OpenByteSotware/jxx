#include <gtest/gtest.h>

#include <type_traits>

#include "beans/jxx.beans.PropertyChangeEvent.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.SerializableI.h"
#include "security/cert/jxx.security.cert.X509Certificate.h"
#include "security/cert/jxx.security.cert.X509Extension.h"

TEST(Java8InheritanceStage2Test, X509CertificateImplementsX509Extension) {
    static_assert(std::is_base_of_v<
        ::jxx::security::cert::X509Extension,
        ::jxx::security::cert::X509Certificate>);

    EXPECT_NE(nullptr, ::jxx::security::cert::X509Extension::Class());
    EXPECT_NE(nullptr, ::jxx::security::cert::X509Certificate::Class());
}

TEST(Java8InheritanceStage2Test, PrintStreamImplementsCloseable) {
    static_assert(std::is_base_of_v<
        ::jxx::io::Closeable,
        ::jxx::io::PrintStream>);

    EXPECT_NE(nullptr, ::jxx::io::PrintStream::Class());
}

TEST(Java8InheritanceStage2Test, PropertyChangeEventInheritsSerializationFromEventObject) {
    static_assert(std::is_base_of_v<
        ::jxx::io::SerializableI,
        ::jxx::beans::PropertyChangeEvent>);

    static_assert(std::is_same_v<
        ::jxx::beans::PropertyChangeEvent::JxxSuper,
        ::jxx::util::EventObject>);

    EXPECT_NE(nullptr, ::jxx::beans::PropertyChangeEvent::Class());
}
