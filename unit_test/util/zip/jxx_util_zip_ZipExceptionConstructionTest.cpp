#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/zip/jxx.util.zip.ZipException.h"

namespace {

TEST(ZipExceptionConstruction, SupportsAllPublicConstructors) {
    const ::jxx::util::zip::ZipException empty;
    const ::jxx::util::zip::ZipException fromCString("bad zip");
    const ::jxx::util::zip::ZipException fromStdString(
        std::string("bad central directory"));
    const ::jxx::util::zip::ZipException fromJxxString(
        ::jxx::NEW<::jxx::lang::String>("bad local header"));

    EXPECT_NE(nullptr, empty.getMessage());
    EXPECT_EQ("bad zip", fromCString.getMessage()->utf8());
    EXPECT_EQ(
        "bad central directory",
        fromStdString.getMessage()->utf8());
    EXPECT_EQ(
        "bad local header",
        fromJxxString.getMessage()->utf8());
}

TEST(ZipExceptionConstruction, PreservesIOExceptionInheritance) {
    const auto exception =
        ::jxx::NEW<::jxx::util::zip::ZipException>("zip error");

    EXPECT_NE(
        nullptr,
        ::jxx::CAST<::jxx::io::IOException>(exception));
}

} // namespace
