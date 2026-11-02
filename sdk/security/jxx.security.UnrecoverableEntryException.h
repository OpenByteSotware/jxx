#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "security/jxx.security.GeneralSecurityException.h"

namespace jxx::security {

class UnrecoverableEntryException
    : public ::jxx::lang::ClassBase<
          UnrecoverableEntryException,
          GeneralSecurityException> {
public:
    using JxxSuper = GeneralSecurityException;
    using Super = ::jxx::lang::ClassBase<UnrecoverableEntryException, JxxSuper>;

    UnrecoverableEntryException();
    explicit UnrecoverableEntryException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit UnrecoverableEntryException(const char* message);
    ~UnrecoverableEntryException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::security
