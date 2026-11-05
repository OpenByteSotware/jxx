#include <gtest/gtest.h>
#include "lang/jxx.lang.ClassLoader.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace {
class TestClassLoader final : public ::jxx::lang::ClassLoader {
public:
    void invokeSetSigners(const ::jxx::Ptr<::jxx::lang::ClassAny>& type) {
        setSigners(type, nullptr);
    }
};
}

TEST(ClassLoaderJava8Parity, SetSignersRejectsNullClass) {
    auto loader = ::jxx::NEW<TestClassLoader>();
    EXPECT_THROW(loader->invokeSetSigners(nullptr), ::jxx::lang::NullPointerException);
}
