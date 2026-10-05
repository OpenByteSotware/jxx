#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::util::zip {

class DataFormatException
    : public ::jxx::lang::ClassBase<DataFormatException, ::jxx::lang::Exception> {
public:
    using JxxSuper = ::jxx::lang::Exception;
    using Super = ::jxx::lang::ClassBase<DataFormatException, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    DataFormatException();
    explicit DataFormatException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit DataFormatException(const char* message);
    explicit DataFormatException(const std::string& message);
    ~DataFormatException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::util::zip
