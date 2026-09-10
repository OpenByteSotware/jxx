#include <future>
#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.PipedInputStream.h"
#include "io/jxx.io.PipedOutputStream.h"
#include "io/jxx.io.PushbackReader.h"
#include "io/jxx.io.SequenceInputStream.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"
TEST(IoRemaining, PipeTransfersAndEnds){auto input=::jxx::NEW<::jxx::io::PipedInputStream>(4);auto output=::jxx::NEW<::jxx::io::PipedOutputStream>(input);auto task=std::async(std::launch::async,[&]{output->write(65);output->close();});EXPECT_EQ(input->read(),65);EXPECT_EQ(input->read(),-1);task.get();}
TEST(IoRemaining, PushbackReaderUnread){auto base=::jxx::NEW<::jxx::io::StringReader>(::jxx::NEW<::jxx::lang::String>("ab"));auto reader=::jxx::NEW<::jxx::io::PushbackReader>(base,2);EXPECT_EQ(reader->read(),'a');reader->unread('x');EXPECT_EQ(reader->read(),'x');EXPECT_EQ(reader->read(),'b');}
TEST(IoRemaining, SequenceReadsInOrder){auto a=::jxx::NEW<::jxx::lang::ByteArrayType>(1);auto b=::jxx::NEW<::jxx::lang::ByteArrayType>(1);(*a)[0]=1;(*b)[0]=2;auto stream=::jxx::NEW<::jxx::io::SequenceInputStream>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(a),::jxx::NEW<::jxx::io::ByteArrayInputStream>(b));EXPECT_EQ(stream->read(),1);EXPECT_EQ(stream->read(),2);EXPECT_EQ(stream->read(),-1);}
