#include <gtest/gtest.h>

#include <type_traits>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Readable.h"

TEST(JxxLangReadableStage, IsAbstractInterface) {
    EXPECT_TRUE((std::is_abstract_v<::jxx::lang::Readable>));
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::lang::Readable>));
}
