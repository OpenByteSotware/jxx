#pragma once

#include "io/jxx.io.InputStreamReader.h"

namespace jxx::lang {
class String;
}

namespace jxx::io {

class File;

class FileReader
    : public ::jxx::lang::ClassBase<
          FileReader,
          InputStreamReader> {
public:
    using JxxSuper = InputStreamReader;

    using Super =
        ::jxx::lang::ClassBase<
            FileReader,
            JxxSuper>;

    explicit FileReader(
        const ::jxx::Ptr<::jxx::lang::String>& fileName);

    explicit FileReader(
        const ::jxx::Ptr<File>& file);

    ~FileReader() override;
};

} // namespace jxx::io
