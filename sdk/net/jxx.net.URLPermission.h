#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"

namespace jxx::net
{
    class URLPermission final : public jxx::lang::Object
    {
    public:
        explicit URLPermission(jxx::Ptr<jxx::lang::String> url);
        URLPermission(jxx::Ptr<jxx::lang::String> url,
                      jxx::Ptr<jxx::lang::String> actions);
        ~URLPermission() override = default;

    public:
        jxx::Ptr<jxx::lang::String> getName() const;
        jxx::Ptr<jxx::lang::String> getActions() const;
        jxx::lang::jbool implies(jxx::Ptr<URLPermission> p) const;

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        jxx::Ptr<jxx::lang::String> url_;
        jxx::Ptr<jxx::lang::String> actions_;
    };
}
