#pragma once

#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::text {

class ParseException
    : public ::jxx::lang::ClassBase<
          ParseException, ::jxx::lang::Exception> {
public:
    using JxxSuper = ::jxx::lang::Exception;
    using Super = ::jxx::lang::ClassBase<ParseException, JxxSuper>;

    ParseException(
        const ::jxx::Ptr<::jxx::lang::String>& message,
        ::jxx::lang::jint errorOffset);
    ParseException(const ParseException& other);

    ::jxx::lang::jint getErrorOffset() const noexcept;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;

private:
    ::jxx::lang::jint errorOffset_;
};

} // namespace jxx::text
