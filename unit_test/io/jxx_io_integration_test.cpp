#include <gtest/gtest.h>
#include "io/jxx.io.BufferedInputStream.h"
#include "io/jxx.io.BufferedOutputStream.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.DataInputStream.h"
#include "io/jxx.io.DataOutputStream.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(IoIntegrationTest, BufferedByteStreamsRoundTripAcrossSmallBuffers) {
 const auto sink=::jxx::NEW<::jxx::io::ByteArrayOutputStream>(); const auto out=::jxx::NEW<::jxx::io::BufferedOutputStream>(sink,3);
 auto bytes=std::make_shared<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(9); for(::jxx::lang::jint i=0;i<9;++i)(*bytes)[i]=static_cast<::jxx::lang::jbyte>(i+1);
 out->write(bytes,0,2); EXPECT_EQ(0,sink->size()); out->write(bytes,2,2); EXPECT_EQ(2,sink->size()); out->write(bytes,4,2); EXPECT_EQ(4,sink->size()); out->write(bytes,6,3); EXPECT_EQ(9,sink->size()); out->flush(); EXPECT_EQ(9,sink->size());
 const auto input=::jxx::NEW<::jxx::io::BufferedInputStream>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(sink->toByteArray()),2);
 auto received=std::make_shared<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(9); EXPECT_EQ(9,input->read(received,0,9)); for(::jxx::lang::jint i=0;i<9;++i)EXPECT_EQ((*bytes)[i],(*received)[i]); EXPECT_EQ(-1,input->read());
}
TEST(IoIntegrationTest, DataStreamsPreservePrimitiveWireValues) {
 const auto sink=::jxx::NEW<::jxx::io::ByteArrayOutputStream>(); const auto output=::jxx::NEW<::jxx::io::DataOutputStream>(sink);
 output->writeBoolean(true); output->writeInt(0x12345678); output->writeLong(static_cast<::jxx::lang::jlong>(0x0102030405060708LL)); output->writeUTF(::jxx::NEW<::jxx::lang::String>("hello")); output->flush();
 const auto input=::jxx::NEW<::jxx::io::DataInputStream>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(sink->toByteArray())); EXPECT_TRUE(input->readBoolean()); EXPECT_EQ(0x12345678,input->readInt()); EXPECT_EQ(static_cast<::jxx::lang::jlong>(0x0102030405060708LL),input->readLong()); EXPECT_EQ("hello",input->readUTF()->utf8());
}
}
