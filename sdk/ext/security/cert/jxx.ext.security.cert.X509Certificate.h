#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::ext::security::cert {

class X509Certificate
    : public ::jxx::lang::ClassBase<
          X509Certificate,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<X509Certificate, JxxSuper>;

    ~X509Certificate() override = default;
    virtual ::jxx::lang::ByteArray getEncoded() const = 0;

protected:
    X509Certificate() = default;
};

} // namespace jxx::ext::security::cert
