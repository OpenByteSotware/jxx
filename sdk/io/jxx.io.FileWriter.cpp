#include "io/jxx.io.FileWriter.h"

#include "io/jxx.io.File.h"
#include "io/jxx.io.FileOutputStream.h"

namespace jxx::io {

FileWriter::FileWriter(
    const ::jxx::Ptr<::jxx::lang::String>& fileName)
    : FileWriter(fileName, false) {
}

FileWriter::FileWriter(
    const ::jxx::Ptr<::jxx::lang::String>& fileName,
    ::jxx::lang::jbool append)
    : Super(
          ::jxx::NEW<FileOutputStream>(
              fileName,
              append)) {
}

FileWriter::FileWriter(
    const ::jxx::Ptr<File>& file)
    : FileWriter(file, false) {
}

FileWriter::FileWriter(
    const ::jxx::Ptr<File>& file,
    ::jxx::lang::jbool append)
    : Super(
          ::jxx::NEW<FileOutputStream>(
              file,
              append)) {
}

FileWriter::~FileWriter() = default;

} // namespace jxx::io
