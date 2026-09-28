#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::security {

class CryptoPrimitive final
    : public ::jxx::lang::ClassBase<
          CryptoPrimitive,
          ::jxx::lang::Object> {
public:
    explicit CryptoPrimitive(
        const ::jxx::Ptr<::jxx::lang::String>& name);

    ::jxx::Ptr<::jxx::lang::String> name() const;

    static ::jxx::Ptr<CryptoPrimitive> MESSAGE_DIGEST();
    static ::jxx::Ptr<CryptoPrimitive> SECURE_RANDOM();
    static ::jxx::Ptr<CryptoPrimitive> BLOCK_CIPHER();
    static ::jxx::Ptr<CryptoPrimitive> STREAM_CIPHER();
    static ::jxx::Ptr<CryptoPrimitive> MAC();
    static ::jxx::Ptr<CryptoPrimitive> KEY_WRAP();
    static ::jxx::Ptr<CryptoPrimitive> PUBLIC_KEY_ENCRYPTION();
    static ::jxx::Ptr<CryptoPrimitive> SIGNATURE();
    static ::jxx::Ptr<CryptoPrimitive> KEY_ENCAPSULATION();
    static ::jxx::Ptr<CryptoPrimitive> KEY_AGREEMENT();

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
};

} // namespace jxx::security
