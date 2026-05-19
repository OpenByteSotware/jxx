#pragma once

#include "jxx.io.InputStreamReader.h"
#include "jxx.io.FileInputStream.h"
#include "jxx.lang.Charset.h"

namespace jxx::io {

class FileReader final : public InputStreamReader {
public:
    explicit FileReader(jxx::Ptr<jxx::lang::String> fileName)
        : InputStreamReader(JXX_NEW<FileInputStream>(fileName), jxx::lang::Charset::defaultCharset()) {}
};

} // namespace jxx::io
