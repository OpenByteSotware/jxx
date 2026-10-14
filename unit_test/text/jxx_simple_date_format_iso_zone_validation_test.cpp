#include <gtest/gtest.h>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.SimpleDateFormat.h"
TEST(SimpleDateFormatIsoZoneParity, RejectsMoreThanThreeXLetters) {
 EXPECT_THROW(::jxx::NEW<::jxx::text::SimpleDateFormat>(::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd XXXX")),::jxx::lang::IllegalArgumentException);
}
