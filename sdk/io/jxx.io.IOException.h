#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::io {

class IOException : public jxx::lang::Exception {
public:
    using JxxSuper = jxx::lang::Exception;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<IOException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    IOException();
    explicit IOException(const jxx::Ptr<jxx::lang::String>& message);
    explicit IOException(const char* message);
    explicit IOException(const std::string& message);

public:
    IOException(const IOException&) = default;
    IOException(IOException&&) noexcept = default;
    IOException& operator=(const IOException&) = default;
    IOException& operator=(IOException&&) noexcept = default;
    ~IOException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
