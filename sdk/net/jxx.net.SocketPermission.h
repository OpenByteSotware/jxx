#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class SocketPermission final : public jxx::lang::Object
    {
    public:
        SocketPermission(jxx::Ptr<jxx::lang::String> host,
                         jxx::Ptr<jxx::lang::String> action);
        ~SocketPermission() override = default;

    public:
        jxx::Ptr<jxx::lang::String> getName() const;
        jxx::Ptr<jxx::lang::String> getActions() const;
        jxx::lang::jbool implies(jxx::Ptr<SocketPermission> p) const;

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        jxx::Ptr<jxx::lang::String> host_;
        jxx::Ptr<jxx::lang::String> action_;
    };
}
