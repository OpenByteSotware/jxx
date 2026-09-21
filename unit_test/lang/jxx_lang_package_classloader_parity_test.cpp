#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassLoader.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.Package.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.URL.h"

namespace {

using ::jxx::lang::ClassLoader;
using ::jxx::lang::IllegalArgumentException;
using ::jxx::lang::NumberFormatException;
using ::jxx::lang::Package;
using ::jxx::lang::String;
using ::jxx::net::URL;

class TestClassLoader final : public ClassLoader {
public:
    TestClassLoader() : ClassLoader(nullptr) {}
    using ClassLoader::definePackage;
};

TEST(PackageParityTest, CompatibleVersionUsesNumericComponents) {
    const auto package = ::jxx::NEW<Package>(
        ::jxx::NEW<String>("example.package"),
        ::jxx::NEW<String>("Example"),
        ::jxx::NEW<String>("2.3.0"));

    EXPECT_TRUE(package->isCompatibleWith(::jxx::NEW<String>("2.2.9")));
    EXPECT_TRUE(package->isCompatibleWith(::jxx::NEW<String>("2.3")));
    EXPECT_FALSE(package->isCompatibleWith(::jxx::NEW<String>("2.3.1")));
}

TEST(PackageParityTest, InvalidVersionComponentsThrowNumberFormatException) {
    const auto package = ::jxx::NEW<Package>(
        ::jxx::NEW<String>("example.package"), nullptr,
        ::jxx::NEW<String>("1.0"));

    EXPECT_THROW(package->isCompatibleWith(::jxx::NEW<String>("1..0")),
                 NumberFormatException);
    EXPECT_THROW(package->isCompatibleWith(::jxx::NEW<String>("1.-1")),
                 NumberFormatException);
}

TEST(PackageParityTest, SealingUsesUrlIdentityContract) {
    const auto sealBase = ::jxx::NEW<URL>(::jxx::NEW<String>("file:/sdk/example.jar"));
    const auto equivalent = ::jxx::NEW<URL>(::jxx::NEW<String>("file:/sdk/example.jar"));
    const auto different = ::jxx::NEW<URL>(::jxx::NEW<String>("file:/sdk/other.jar"));
    const auto package = ::jxx::NEW<Package>(
        ::jxx::NEW<String>("example.package"), nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, sealBase);

    EXPECT_TRUE(package->isSealed());
    EXPECT_TRUE(package->isSealed(equivalent));
    EXPECT_FALSE(package->isSealed(different));
    EXPECT_FALSE(package->isSealed(nullptr));
}

TEST(ClassLoaderPackageParityTest, DefinesMetadataAndRejectsDuplicatePackage) {
    const auto loader = ::jxx::NEW<TestClassLoader>();
    const auto name = ::jxx::NEW<String>("example.loader.package");
    const auto sealBase = ::jxx::NEW<URL>(::jxx::NEW<String>("file:/sdk/loader.jar"));

    const auto package = loader->definePackage(
        name,
        ::jxx::NEW<String>("Specification"),
        ::jxx::NEW<String>("1.2.3"),
        ::jxx::NEW<String>("Vendor"),
        ::jxx::NEW<String>("Implementation"),
        ::jxx::NEW<String>("9"),
        ::jxx::NEW<String>("Implementation Vendor"),
        sealBase);

    ASSERT_NE(nullptr, package);
    EXPECT_TRUE(package->isSealed(sealBase));
    EXPECT_TRUE(package->isCompatibleWith(::jxx::NEW<String>("1.2")));
    EXPECT_THROW(loader->definePackage(name), IllegalArgumentException);
}

} // namespace
