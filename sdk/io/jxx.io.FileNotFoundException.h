#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.IOException.h"

namespace jxx::io {

class FileNotFoundException : public IOException {
public:
    using JxxSuper = IOException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<FileNotFoundException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    FileNotFoundException();
    explicit FileNotFoundException(const jxx::Ptr<jxx::lang::String>& message);
    explicit FileNotFoundException(const char* message);
    explicit FileNotFoundException(const std::string& message);

public:
    FileNotFoundException(const FileNotFoundException&) = default;
    FileNotFoundException(FileNotFoundException&&) noexcept = default;
    FileNotFoundException& operator=(const FileNotFoundException&) = default;
    FileNotFoundException& operator=(FileNotFoundException&&) noexcept = default;
    ~FileNotFoundException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
