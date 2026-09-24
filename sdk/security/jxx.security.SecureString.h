#pragma once

#include <cstddef>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::security {

class SecureString final
    : public ::jxx::lang::ClassBase<
          SecureString,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SecureString, JxxSuper>;

    SecureString();
    explicit SecureString(const ::jxx::Ptr<::jxx::lang::String>& value);
    explicit SecureString(const ::jxx::lang::ByteArray& value);
    SecureString(
        const ::jxx::lang::ByteArray& value,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length);

    SecureString(const SecureString&) = delete;
    SecureString& operator=(const SecureString&) = delete;
    SecureString(SecureString&& other) noexcept;
    SecureString& operator=(SecureString&& other) noexcept;
    ~SecureString() override;

    void assign(const ::jxx::Ptr<::jxx::lang::String>& value);
    void assign(const ::jxx::lang::ByteArray& value);
    void assign(
        const ::jxx::lang::ByteArray& value,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length);
    void clear() noexcept;
    void append(::jxx::lang::jbyte value);
    ::jxx::lang::jint length() const noexcept;
    ::jxx::lang::jbool isEmpty() const noexcept;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
    ::jxx::lang::ByteArray toByteArray() const;

private:
    void assignNative_(const char* data, std::size_t length);
    void moveFrom_(SecureString&& other) noexcept;
    void secureWipe_() noexcept;

    std::vector<char> buffer_;
};

} // namespace jxx::security
