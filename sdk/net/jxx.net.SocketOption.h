#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang
{
    class Class;
}

namespace jxx::net
{
    template<typename T>
    class SocketOption : public jxx::lang::Object
    {
    public:
        ~SocketOption() override = default;

    public:
        virtual jxx::Ptr<jxx::lang::String> name() const = 0;
        virtual jxx::Ptr<jxx::lang::Class> type() const = 0;
    };

    class BasicSocketOption final : public SocketOption<jxx::lang::Object>
    {
    public:
        explicit BasicSocketOption(jxx::Ptr<jxx::lang::String> name);
        ~BasicSocketOption() override = default;

    public:
        jxx::Ptr<jxx::lang::String> name() const override;
        jxx::Ptr<jxx::lang::Class> type() const override;
        jxx::Ptr<jxx::lang::String> toString() const override;

    private:
        jxx::Ptr<jxx::lang::String> name_;
    };
}
