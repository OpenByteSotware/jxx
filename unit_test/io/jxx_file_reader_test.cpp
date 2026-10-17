#include <cstdio>
#include <fstream>
#include <string>

#include <gtest/gtest.h>

#include "io/jxx.io.File.h"
#include "io/jxx.io.FileInputStream.h"
#include "io/jxx.io.FileReader.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace {

class FileReaderParityTest : public testing::Test {
protected:
    void SetUp() override {
        path_ = "jxx_file_reader_descriptor_test.txt";
        std::ofstream output(path_, std::ios::binary);
        output << "descriptor";
    }

    void TearDown() override {
        std::remove(path_.c_str());
    }

    std::string path_;
};

TEST_F(FileReaderParityTest, ConstructsFromFileDescriptor)
{
    const auto path = ::jxx::NEW<::jxx::lang::String>(path_);
    const auto input = ::jxx::NEW<::jxx::io::FileInputStream>(path);
    const auto reader = ::jxx::NEW<::jxx::io::FileReader>(input->getFD());

    EXPECT_EQ(reader->read(), static_cast<::jxx::lang::jint>('d'));
    reader->close();
}

TEST_F(FileReaderParityTest, NullDescriptorThrows)
{
    const ::jxx::Ptr<::jxx::io::FileDescriptor> descriptor;
    EXPECT_THROW(
        (::jxx::NEW<::jxx::io::FileReader>(descriptor)),
        ::jxx::lang::NullPointerException);
}

} // namespace
