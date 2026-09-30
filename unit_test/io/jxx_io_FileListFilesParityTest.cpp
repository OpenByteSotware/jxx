#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include "io/jxx.io.File.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(FileListFilesParity, ReturnsJxxFileArrayForDirectory) {
    const auto root=std::filesystem::temp_directory_path()/"jxx_file_list_parity";
    std::filesystem::remove_all(root);std::filesystem::create_directories(root);
    std::ofstream(root/"a.txt")<<"a";std::ofstream(root/"b.txt")<<"b";
    const auto file=::jxx::NEW<::jxx::io::File>(::jxx::NEW<::jxx::lang::String>(root.u8string()));
    const auto children=file->listFiles();ASSERT_NE(nullptr,children);EXPECT_EQ(2,children->length);
    std::filesystem::remove_all(root);
}
TEST(FileListFilesParity, ReturnsNullForNonDirectory) {
    const auto path=std::filesystem::temp_directory_path()/"jxx_file_list_regular.txt";std::ofstream(path)<<"x";
    const auto file=::jxx::NEW<::jxx::io::File>(::jxx::NEW<::jxx::lang::String>(path.u8string()));
    EXPECT_EQ(nullptr,file->listFiles());std::filesystem::remove(path);
}
}
