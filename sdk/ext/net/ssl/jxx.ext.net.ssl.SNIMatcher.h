#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::net::ssl {
class SNIServerName;
class SNIMatcher
    : public ::jxx::lang::ClassBase<SNIMatcher, ::jxx::lang::Object> {
public:
    explicit SNIMatcher(::jxx::lang::jint type);
    ~SNIMatcher() override = default;
    ::jxx::lang::jint getType() const;
    virtual ::jxx::lang::jbool matches(
        const ::jxx::Ptr<SNIServerName>& serverName) const = 0;
private:
    ::jxx::lang::jint type_;
};
} // namespace jxx::ext::net::ssl
