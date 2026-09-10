#include "io/jxx.io.FileReader.h"

#include "io/jxx.io.File.h"
#include "io/jxx.io.FileInputStream.h"

namespace jxx::io {

FileReader::FileReader(
    const ::jxx::Ptr<::jxx::lang::String>& fileName) : Super(
          ::jxx::NEW<FileInputStream>(
              fileName)) {
}

FileReader::FileReader(
    const ::jxx::Ptr<File>& file)
    : Super(
          ::jxx::NEW<FileInputStream>(
              file)) {
}

FileReader::~FileReader() = default;

} // namespace jxx::io
