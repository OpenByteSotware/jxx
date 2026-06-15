#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"

namespace jxx::nio::charset
{
    class CodingErrorAction final : public jxx::lang::Object
    {
    public:
        static jxx::Ptr<CodingErrorAction> IGNORE;
        static jxx::Ptr<CodingErrorAction> REPLACE;
        static jxx::Ptr<CodingErrorAction> REPORT;

    public:
        explicit CodingErrorAction(jxx::Ptr<jxx::lang::String> name);
        ~CodingErrorAction() override = default;

    public:
        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        jxx::Ptr<jxx::lang::String> name_;
    };
}
