#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::security {

class GeneralSecurityException
    : public ::jxx::lang::ClassBase<
          GeneralSecurityException,
          ::jxx::lang::Exception> {
public:
    using JxxSuper = ::jxx::lang::Exception;
    using Super = ::jxx::lang::ClassBase<
        GeneralSecurityException,
        JxxSuper>;

    GeneralSecurityException();
    explicit GeneralSecurityException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit GeneralSecurityException(const char* message);
    ~GeneralSecurityException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object>
    cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::security
