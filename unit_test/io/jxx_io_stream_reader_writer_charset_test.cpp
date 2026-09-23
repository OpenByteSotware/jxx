#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.InputStreamReader.h"
#include "io/jxx.io.OutputStreamWriter.h"
#include "io/jxx.io.UnsupportedEncodingException.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(StreamReaderWriterCharsetTest, RejectsUnsupportedCharsetName) {
 const auto bad=::jxx::NEW<::jxx::lang::String>("NO-SUCH-CHARSET");
 EXPECT_THROW(::jxx::NEW<::jxx::io::InputStreamReader>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(std::make_shared<::jxx::lang::JxxArray<::jxx::lang::jbyte,1U>>(0)),bad),::jxx::io::UnsupportedEncodingException);
 EXPECT_THROW(::jxx::NEW<::jxx::io::OutputStreamWriter>(::jxx::NEW<::jxx::io::ByteArrayOutputStream>(),bad),::jxx::io::UnsupportedEncodingException);
}
TEST(StreamReaderWriterCharsetTest, Iso88591RoundTripsExtendedCharacter) {
 const auto sink=::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
 const auto encoding=::jxx::NEW<::jxx::lang::String>("ISO-8859-1");
 const auto writer=::jxx::NEW<::jxx::io::OutputStreamWriter>(sink,encoding);
 writer->write(::jxx::NEW<::jxx::lang::String>(std::u16string(1,u'\u00E9')),0,1); writer->flush();
 ASSERT_EQ(1,sink->size());
 const auto reader=::jxx::NEW<::jxx::io::InputStreamReader>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(sink->toByteArray()),encoding);
 EXPECT_EQ(static_cast<::jxx::lang::jint>(u'\u00E9'),reader->read()); EXPECT_EQ(-1,reader->read());
}
}
