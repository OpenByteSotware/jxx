#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

#if defined(BIG_ENDIAN)
#undef BIG_ENDIAN
#endif
#if defined(LITTLE_ENDIAN)
#undef LITTLE_ENDIAN
#endif

namespace jxx::lang {
    class String;
}
namespace jxx::nio
{
    class ByteOrder final : public jxx::lang::ClassBase<ByteOrder, jxx::lang::Object>
    {
    public:
        using JxxSuper = jxx::lang::Object;
        using Super = jxx::lang::ClassBase<ByteOrder, JxxSuper>;

        static jxx::Ptr<ByteOrder> BIG_ENDIAN;
        static jxx::Ptr<ByteOrder> LITTLE_ENDIAN;

    public:
        explicit ByteOrder(const jxx::Ptr<jxx::lang::String> name);
        ~ByteOrder() override = default;

    public:
        static jxx::Ptr<ByteOrder> nativeOrder();

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        jxx::Ptr<jxx::lang::String> name_;
    };
}
