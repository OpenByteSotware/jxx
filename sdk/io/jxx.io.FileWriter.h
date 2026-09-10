#pragma once

#include "io/jxx.io.OutputStreamWriter.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::io {

class File;

class FileWriter
    : public ::jxx::lang::ClassBase<
          FileWriter,
          OutputStreamWriter> {
public:
    using JxxSuper = OutputStreamWriter;

    using Super =
        ::jxx::lang::ClassBase<
            FileWriter,
            JxxSuper>;

    explicit FileWriter(
        const ::jxx::Ptr<::jxx::lang::String>& fileName);

    FileWriter(
        const ::jxx::Ptr<::jxx::lang::String>& fileName,
        ::jxx::lang::jbool append);

    explicit FileWriter(
        const ::jxx::Ptr<File>& file);

    FileWriter(
        const ::jxx::Ptr<File>& file,
        ::jxx::lang::jbool append);

    ~FileWriter() override;
};

} // namespace jxx::io
