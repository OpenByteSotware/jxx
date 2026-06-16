#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class StandardProtocolFamily final : public jxx::lang::Object
    {
    public:
        static jxx::Ptr<StandardProtocolFamily> INET;
        static jxx::Ptr<StandardProtocolFamily> INET6;

    public:
        explicit StandardProtocolFamily(jxx::Ptr<jxx::lang::String> name);
        ~StandardProtocolFamily() override = default;

    public:
        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        jxx::Ptr<jxx::lang::String> name_;
    };
}
