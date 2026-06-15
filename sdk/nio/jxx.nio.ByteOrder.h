#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {
    class String;
}
namespace jxx::nio
{
    class ByteOrder final : public jxx::lang::Object
    {
    public:
        static jxx::Ptr<ByteOrder> BIG_ENDIAN;
        static jxx::Ptr<ByteOrder> LITTLE_ENDIAN;

    public:
        explicit ByteOrder(jxx::Ptr<jxx::lang::String> name);
        ~ByteOrder() override = default;

    public:
        static jxx::Ptr<ByteOrder> nativeOrder();

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        jxx::Ptr<jxx::lang::String> name_;
    };
}
