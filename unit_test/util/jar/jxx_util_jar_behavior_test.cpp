#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "util/jar/jxx.util.jar.Attributes.h"
#include "util/jar/jxx.util.jar.JarInputStream.h"
#include "util/jar/jxx.util.jar.JarOutputStream.h"
#include "util/jar/jxx.util.jar.Manifest.h"

namespace {

TEST(JxxUtilJarBehaviorTest, ManifestRoundTripsMainAndEntryAttributes)
{
    const auto manifest = ::jxx::NEW<::jxx::util::jar::Manifest>();
    manifest->getMainAttributes()->putValue(
        ::jxx::NEW<::jxx::lang::String>("Manifest-Version"),
        ::jxx::NEW<::jxx::lang::String>("1.0"));
    manifest->getMainAttributes()->putValue(
        ::jxx::NEW<::jxx::lang::String>("Main-Class"),
        ::jxx::NEW<::jxx::lang::String>("example.Main"));
    const auto section = ::jxx::NEW<::jxx::util::jar::Attributes>();
    section->putValue(::jxx::NEW<::jxx::lang::String>("Sealed"),
        ::jxx::NEW<::jxx::lang::String>("true"));
    manifest->getEntries()->put(
        ::jxx::NEW<::jxx::lang::String>("example/Main.class"), section);

    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    manifest->write(bytes);
    const auto restored = ::jxx::NEW<::jxx::util::jar::Manifest>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));

    EXPECT_EQ("example.Main", restored->getMainAttributes()->getValue(
        ::jxx::NEW<::jxx::lang::String>("Main-Class"))->utf8());
    ASSERT_NE(nullptr, restored->getAttributes(
        ::jxx::NEW<::jxx::lang::String>("example/Main.class")));
    EXPECT_EQ("true", restored->getAttributes(
        ::jxx::NEW<::jxx::lang::String>("example/Main.class"))->getValue(
            ::jxx::NEW<::jxx::lang::String>("Sealed"))->utf8());
}

TEST(JxxUtilJarBehaviorTest, JarStreamsWriteAndConsumeLeadingManifest)
{
    const auto manifest = ::jxx::NEW<::jxx::util::jar::Manifest>();
    manifest->getMainAttributes()->putValue(
        ::jxx::NEW<::jxx::lang::String>("Manifest-Version"),
        ::jxx::NEW<::jxx::lang::String>("1.0"));
    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    {
        const auto output = ::jxx::NEW<::jxx::util::jar::JarOutputStream>(
            bytes, manifest);
        output->finish();
    }
    const auto input = ::jxx::NEW<::jxx::util::jar::JarInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    ASSERT_NE(nullptr, input->getManifest());
    EXPECT_EQ("1.0", input->getManifest()->getMainAttributes()->getValue(
        ::jxx::NEW<::jxx::lang::String>("Manifest-Version"))->utf8());
    EXPECT_EQ(nullptr, input->getNextJarEntry());
}

TEST(JxxUtilJarBehaviorTest, AttributesEqualityUsesNamesAndValues)
{
    const auto left = ::jxx::NEW<::jxx::util::jar::Attributes>();
    const auto right = ::jxx::NEW<::jxx::util::jar::Attributes>();
    left->putValue(::jxx::NEW<::jxx::lang::String>("Main-Class"),
        ::jxx::NEW<::jxx::lang::String>("example.Main"));
    right->putValue(::jxx::NEW<::jxx::lang::String>("main-class"),
        ::jxx::NEW<::jxx::lang::String>("example.Main"));
    EXPECT_TRUE(left->equals(right));
    EXPECT_EQ(left->hashCode(), right->hashCode());
    right->putValue(::jxx::NEW<::jxx::lang::String>("main-class"),
        ::jxx::NEW<::jxx::lang::String>("other.Main"));
    EXPECT_FALSE(left->equals(right));
}

} // namespace
