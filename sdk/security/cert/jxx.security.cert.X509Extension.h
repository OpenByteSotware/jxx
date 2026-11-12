#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Set.h"

namespace jxx::security::cert {

class X509Extension
    : public ::jxx::lang::InterfaceBase<X509Extension> {
public:
    ~X509Extension() override = default;

    virtual ::jxx::lang::jbool hasUnsupportedCriticalExtension() const = 0;
    virtual ::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>>
    getCriticalExtensionOIDs() const = 0;
    virtual ::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>>
    getNonCriticalExtensionOIDs() const = 0;
    virtual ::jxx::lang::ByteArray getExtensionValue(
        const ::jxx::Ptr<::jxx::lang::String>& oid) const = 0;
};

} // namespace jxx::security::cert
