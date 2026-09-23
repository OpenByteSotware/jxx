#include <gtest/gtest.h>

#include <cstdio>
#include <type_traits>

#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
#include "lang/jxx.lang.String.h"

namespace {
static_assert(std::is_nothrow_destructible_v<::jxx::io::FileInputStream>);
static_assert(std::is_nothrow_destructible_v<::jxx::io::FileOutputStream>);
TEST(FileStreamDestructorSafetyTest, ScopeExitClosesOwnedStreams) {
    const auto path=::jxx::NEW<::jxx::lang::String>("jxx_file_stream_destructor_safety.tmp");
    std::remove(path->utf8().c_str());
    {const auto output=::jxx::NEW<::jxx::io::FileOutputStream>(path);output->write(7);}
    {const auto input=::jxx::NEW<::jxx::io::FileInputStream>(path);EXPECT_EQ(7,input->read());}
    std::remove(path->utf8().c_str());
}
}
