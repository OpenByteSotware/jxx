#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassLoader.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Package.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/jxx.util.Map.h"

namespace {
using ::jxx::lang::ClassLoader;
using ::jxx::lang::IllegalArgumentException;
using ::jxx::lang::Package;
using ::jxx::lang::String;
using ::jxx::lang::System;

class TestClassLoader final : public ClassLoader {
public:
    using ClassLoader::ClassLoader;
    using ClassLoader::definePackage;
};

TEST(SystemCompletionParityTest, EnvironmentSnapshotContainsNamedValues) {
    const auto environment = System::getenv();
    ASSERT_NE(nullptr, environment);
    const auto path = ::jxx::NEW<String>("PATH");
    const auto named = System::getenv(path);
    const auto mapped = environment->get(::jxx::CAST<::jxx::lang::Object>(path));
    if (named == nullptr) {
        EXPECT_EQ(nullptr, mapped);
    } else {
        ASSERT_NE(nullptr, mapped);
        EXPECT_EQ(named->utf8(), mapped->utf8());
    }
}

TEST(PackageParityTest, VersionCompatibilityUsesNumericComponents) {
    const auto packageValue = ::jxx::NEW<Package>(
        ::jxx::NEW<String>("jxx.test.versioned"),
        nullptr,
        ::jxx::NEW<String>("2.1.0"));
    EXPECT_TRUE(packageValue->isCompatibleWith(::jxx::NEW<String>("2.0.9")));
    EXPECT_TRUE(packageValue->isCompatibleWith(::jxx::NEW<String>("2.1")));
    EXPECT_FALSE(packageValue->isCompatibleWith(::jxx::NEW<String>("2.1.1")));
}

TEST(ClassLoaderPackageParityTest, PackageOwnershipIsPerLoader) {
    const auto first = ::jxx::NEW<TestClassLoader>(
        ::jxx::Ptr<ClassLoader>{});
    const auto second = ::jxx::NEW<TestClassLoader>(
        ::jxx::Ptr<ClassLoader>{});
    const auto name = ::jxx::NEW<String>("jxx.test.loader.package");

    const auto firstPackage = first->definePackage(name);
    const auto secondPackage = second->definePackage(name);
    EXPECT_NE(firstPackage.get(), secondPackage.get());
    EXPECT_EQ(firstPackage.get(), first->getPackage(name).get());
    EXPECT_EQ(secondPackage.get(), second->getPackage(name).get());
}

TEST(ClassLoaderPackageParityTest, DuplicateDefinitionInOneLoaderIsRejected) {
    const auto loader = ::jxx::NEW<TestClassLoader>(
        ::jxx::Ptr<ClassLoader>{});
    const auto name = ::jxx::NEW<String>("jxx.test.loader.duplicate");
    loader->definePackage(name);
    EXPECT_THROW(loader->definePackage(name), IllegalArgumentException);
}

TEST(ClassLoaderAssertionParityTest, ClassAndPackageOverridesTakePrecedence) {
    const auto loader = ::jxx::NEW<TestClassLoader>(
        ::jxx::Ptr<ClassLoader>{});
    const auto type = String::Class();
    loader->setDefaultAssertionStatus(false);
    loader->setPackageAssertionStatus(::jxx::NEW<String>("jxx.lang"), true);
    EXPECT_TRUE(loader->desiredAssertionStatus(type));
    loader->setClassAssertionStatus(type->getName(), false);
    EXPECT_FALSE(loader->desiredAssertionStatus(type));
    loader->clearAssertionStatus();
    EXPECT_FALSE(loader->desiredAssertionStatus(type));
}

} // namespace
