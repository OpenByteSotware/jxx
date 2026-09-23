#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileOutputStream.h"
namespace { TEST(FileStreamHeaderCompileTest, HeadersExposeCompleteStreamTypesWithoutCstdio) { EXPECT_TRUE((std::is_destructible_v<::jxx::io::FileInputStream>)); EXPECT_TRUE((std::is_destructible_v<::jxx::io::FileOutputStream>)); } }
