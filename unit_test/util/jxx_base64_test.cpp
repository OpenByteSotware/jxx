#include <gtest/gtest.h>
#include "util/jxx.util.Base64.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
TEST(JxxBase64, BasicUrlMimeAndPadding)
{
	auto a = ::jxx::NEW<::jxx::lang::ByteArrayType>(3); 
	(*a)[0] = 'f'; (*a)[1] = 'o'; (*a)[2] = 'o';
	auto e = ::jxx::util::Base64::getEncoder(); 
	EXPECT_EQ(e->encodeToString(a)->utf8(), "Zm9v");
	auto d = ::jxx::util::Base64::getDecoder()->decode(e->encode(a)); ASSERT_EQ(d->length, 3); EXPECT_EQ((*d)[0], 'f'); auto one = ::jxx::NEW<::jxx::lang::ByteArrayType>(1); (*one)[0] = 'f'; EXPECT_EQ(e->withoutPadding()->encodeToString(one)->utf8(), "Zg");
}
TEST(JxxBase64, MimeIgnoresNonAlphabet)
{
	auto s = ::jxx::NEW<::jxx::lang::String>("Z m\r\n9v"); 
	auto d = ::jxx::util::Base64::getMimeDecoder()->decode(s); EXPECT_EQ(d->length, 3);
}
TEST(JxxBase64, InvalidInputAndSmallDestination)
{
	auto s = ::jxx::NEW<::jxx::lang::String>("!");
	EXPECT_THROW(::jxx::util::Base64::getDecoder()->decode(s),
		::jxx::lang::IllegalArgumentException);
}
