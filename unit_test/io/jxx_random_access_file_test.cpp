#include <gtest/gtest.h>
#include <cstdio>
#include <string>

#include "jxx.initialize.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.DataInput.h"
#include "io/jxx.io.DataOutput.h"
#include "io/jxx.io.EOFException.h"
#include "io/jxx.io.RandomAccessFile.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"

namespace {

::jxx::Ptr<::jxx::lang::String> text(const std::string& value) {
    return ::jxx::NEW<::jxx::lang::String>(value);
}

std::string temporaryPath() {
    char name[L_tmpnam];
    std::tmpnam(name);
    return std::string(name) + ".jxx_raf";
}

TEST(RandomAccessFileParity, HasRequiredHierarchy)
{
    ::jxx::initialize();
    const auto path = temporaryPath();
    auto file = ::jxx::NEW<::jxx::io::RandomAccessFile>(text(path), text("rw"));
    EXPECT_NE(::jxx::CAST<::jxx::io::DataInput>(file), nullptr);
    EXPECT_NE(::jxx::CAST<::jxx::io::DataOutput>(file), nullptr);
    EXPECT_NE(::jxx::CAST<::jxx::io::Closeable>(file), nullptr);
    file->close();
    std::remove(path.c_str());
}

TEST(RandomAccessFileParity, PrimitiveAndUtfRoundTrip)
{
    ::jxx::initialize();
    const auto path = temporaryPath();
    auto file = ::jxx::NEW<::jxx::io::RandomAccessFile>(text(path), text("rw"));
    file->writeBoolean(true);
    file->writeByte(-2);
    file->writeShort(0x1234);
    file->writeChar(0x2192);
    file->writeInt(0x12345678);
    file->writeLong(static_cast<::jxx::lang::jlong>(0x1020304050607080LL));
    file->writeFloat(1.25F);
    file->writeDouble(-2.5);
    const std::string utfValue("A\0\xC3\xA9", 4);
    file->writeUTF(text(utfValue));
    file->seek(0);
    EXPECT_TRUE(file->readBoolean());
    EXPECT_EQ(file->readByte(), -2);
    EXPECT_EQ(file->readUnsignedShort(), 0x1234);
    EXPECT_EQ(file->readChar(), static_cast<::jxx::lang::jchar>(0x2192));
    EXPECT_EQ(file->readInt(), 0x12345678);
    EXPECT_EQ(file->readLong(), static_cast<::jxx::lang::jlong>(0x1020304050607080LL));
    EXPECT_FLOAT_EQ(file->readFloat(), 1.25F);
    EXPECT_DOUBLE_EQ(file->readDouble(), -2.5);
    EXPECT_EQ(file->readUTF()->utf8(), utfValue);
    file->close();
    std::remove(path.c_str());
}

TEST(RandomAccessFileParity, SeekLengthTruncateAndExtend)
{
    ::jxx::initialize();
    const auto path = temporaryPath();
    auto file = ::jxx::NEW<::jxx::io::RandomAccessFile>(text(path), text("rw"));
    file->writeInt(1);
    EXPECT_EQ(file->length(), 4);
    file->setLength(2);
    EXPECT_EQ(file->length(), 2);
    EXPECT_EQ(file->getFilePointer(), 2);
    file->setLength(8);
    EXPECT_EQ(file->length(), 8);
    file->seek(7);
    EXPECT_EQ(file->readUnsignedByte(), 0);
    file->close();
    std::remove(path.c_str());
}

TEST(RandomAccessFileParity, ReadOnlyRejectsWriteAndEofThrows)
{
    ::jxx::initialize();
    const auto path = temporaryPath();
    {
        auto output = ::jxx::NEW<::jxx::io::RandomAccessFile>(text(path), text("rw"));
        output->writeByte(1);
    }
    auto input = ::jxx::NEW<::jxx::io::RandomAccessFile>(text(path), text("r"));
    EXPECT_EQ(input->readUnsignedByte(), 1);
    EXPECT_THROW(input->readByte(), ::jxx::io::EOFException);
    EXPECT_THROW(input->writeByte(2), ::jxx::io::IOException);
    input->close();
    std::remove(path.c_str());
}

TEST(RandomAccessFileParity, InvalidModesAndNegativeValuesFail)
{
    ::jxx::initialize();
    const auto path = temporaryPath();
    EXPECT_THROW(
        ::jxx::NEW<::jxx::io::RandomAccessFile>(text(path), text("w")),
        ::jxx::lang::IllegalArgumentException);
    auto file = ::jxx::NEW<::jxx::io::RandomAccessFile>(text(path), text("rw"));
    EXPECT_THROW(file->seek(-1), ::jxx::io::IOException);
    EXPECT_THROW(file->setLength(-1), ::jxx::lang::IllegalArgumentException);
    file->close();
    std::remove(path.c_str());
}

} // namespace
