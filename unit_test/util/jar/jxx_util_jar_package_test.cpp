#include <gtest/gtest.h>
#include <type_traits>
#include "jxx.initialize.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jar/jxx.util.jar.Attributes.h"
#include "util/jar/jxx.util.jar.JarEntry.h"
#include "util/jar/jxx.util.jar.JarException.h"
#include "util/jar/jxx.util.jar.JarFile.h"
#include "util/jar/jxx.util.jar.JarInputStream.h"
#include "util/jar/jxx.util.jar.JarOutputStream.h"
#include "util/jar/jxx.util.jar.Manifest.h"
#include "util/jar/jxx.util.jar.Pack200.h"
namespace {
TEST(JxxUtilJarPackageTest, HasJava8Hierarchy){
 EXPECT_TRUE((std::is_base_of_v<::jxx::util::zip::ZipEntry,::jxx::util::jar::JarEntry>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::util::zip::ZipFile,::jxx::util::jar::JarFile>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::util::zip::ZipInputStream,::jxx::util::jar::JarInputStream>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::util::zip::ZipOutputStream,::jxx::util::jar::JarOutputStream>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::util::zip::ZipException,::jxx::util::jar::JarException>));
}
TEST(JxxUtilJarPackageTest, AttributesNamesAreCaseInsensitive){
 auto a=::jxx::NEW<::jxx::util::jar::Attributes>();
 a->putValue(::jxx::NEW<::jxx::lang::String>("Main-Class"),::jxx::NEW<::jxx::lang::String>("example.Main"));
 EXPECT_EQ("example.Main",a->getValue(::jxx::NEW<::jxx::lang::String>("main-class"))->utf8());
}
TEST(JxxUtilJarPackageTest, RegistersPublicTypes){
 ::jxx::initialize();
 EXPECT_NE(nullptr,::jxx::util::jar::Attributes::Class());
 EXPECT_NE(nullptr,::jxx::util::jar::Manifest::Class());
 EXPECT_NE(nullptr,::jxx::util::jar::JarFile::Class());
 EXPECT_NE(nullptr,::jxx::util::jar::Pack200::Packer::Class());
}
TEST(JxxUtilJarPackageTest, Pack200FailsExplicitlyUntilCodecExists){
 EXPECT_THROW((void)::jxx::util::jar::Pack200::newPacker(),::jxx::lang::UnsupportedOperationException);
}
}
