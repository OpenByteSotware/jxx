#pragma once

#include <stdexcept>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class URISyntaxException final : public std::runtime_error
    {
    public:
        URISyntaxException(jxx::Ptr<jxx::lang::String> input,
                           jxx::Ptr<jxx::lang::String> reason,
                           jxx::lang::jint index);
        URISyntaxException(jxx::Ptr<jxx::lang::String> input,
                           jxx::Ptr<jxx::lang::String> reason);
        ~URISyntaxException() override = default;

    public:
        jxx::Ptr<jxx::lang::String> getInput() const;
        jxx::Ptr<jxx::lang::String> getReason() const;
        jxx::lang::jint getIndex() const noexcept;
        jxx::Ptr<jxx::lang::String> getMessage() const;

    private:
        jxx::Ptr<jxx::lang::String> input_;
        jxx::Ptr<jxx::lang::String> reason_;
        jxx::lang::jint index_ = -1;
    };
}
