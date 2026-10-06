#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.IOException.h"


#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class HttpRetryException : public ::jxx::lang::ClassBase<HttpRetryException, ::jxx::io::IOException>
    {
    public:
        using JxxSuper = ::jxx::io::IOException;
        using Super = ::jxx::lang::ClassBase<HttpRetryException, JxxSuper>;

        HttpRetryException(const jxx::Ptr<jxx::lang::String> detail,
                           jxx::lang::jint code);
        HttpRetryException(const jxx::Ptr<jxx::lang::String> detail,
                           jxx::lang::jint code,
                           jxx::Ptr<jxx::lang::String> location);
        ~HttpRetryException() override = default;

    public:
        jxx::lang::jint responseCode() const noexcept;
        jxx::Ptr<jxx::lang::String> getReason() const;
        jxx::Ptr<jxx::lang::String> getLocation() const;

    private:
        jxx::Ptr<jxx::lang::String> detail_;
        jxx::lang::jint code_ = 0;
        jxx::Ptr<jxx::lang::String> location_;
    };
}
