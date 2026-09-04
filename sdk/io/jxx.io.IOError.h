#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Error.h"

namespace jxx::io {

class IOError : public jxx::lang::Error {
public:
    using JxxSuper = jxx::lang::Error;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IOError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    explicit IOError(const jxx::Ptr<jxx::lang::Throwable>& cause);

    IOError(const IOError&) = default;
    IOError(IOError&&) noexcept = default;
    IOError& operator=(const IOError&) = default;
    IOError& operator=(IOError&&) noexcept = default;
    ~IOError() override = default;

    jxx::Ptr<jxx::lang::Throwable> getCause() const;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;

private:
    jxx::Ptr<jxx::lang::Throwable> cause_;
};

} // namespace jxx::io
