#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.ObjectInput.h"
#include "io/jxx.io.ObjectOutput.h"
#include "lang/jxx.lang.AutoCloseable.h"

TEST(IoCloseableInheritanceParity, CloseableExtendsLangAutoCloseable) {
    static_assert(std::is_base_of_v<::jxx::lang::AutoCloseable, ::jxx::io::Closeable>);
    SUCCEED();
}

TEST(IoCloseableInheritanceParity, ObjectStreamsUseLangAutoCloseable) {
    static_assert(std::is_base_of_v<::jxx::lang::AutoCloseable, ::jxx::io::ObjectInput>);
    static_assert(std::is_base_of_v<::jxx::lang::AutoCloseable, ::jxx::io::ObjectOutput>);
    SUCCEED();
}
