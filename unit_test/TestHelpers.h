#pragma once

#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::tests {

inline std::string utf8(const jxx::Ptr<jxx::lang::String>& s) {
    return s ? s->utf8() : std::string();
}

inline void ExpectStringEq(const jxx::Ptr<jxx::lang::String>& actual, const char* expected) {
    ASSERT_TRUE(actual != nullptr);
    EXPECT_EQ(actual->utf8(), std::string(expected));
}

inline void ExpectStringContains(const jxx::Ptr<jxx::lang::String>& actual, const char* needle) {
    ASSERT_TRUE(actual != nullptr);
    EXPECT_NE(actual->utf8().find(needle), std::string::npos);
}

} // namespace jxx::tests
