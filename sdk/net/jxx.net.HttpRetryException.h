#pragma once

#include <stdexcept>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class HttpRetryException final : public std::runtime_error
    {
    public:
        HttpRetryException(jxx::Ptr<jxx::lang::String> detail,
                           jxx::lang::jint code);
        HttpRetryException(jxx::Ptr<jxx::lang::String> detail,
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
