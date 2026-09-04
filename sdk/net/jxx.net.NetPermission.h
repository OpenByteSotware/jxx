#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class NetPermission final : public jxx::lang::ClassBase<NetPermission, jxx::lang::Object> {
public:
    using JavaSuper = jxx::lang::Object;
    using Super = jxx::lang::ClassBase<NetPermission, jxx::lang::Object>;

    public:
        explicit NetPermission(const jxx::Ptr<jxx::lang::String>& name);
        NetPermission(const jxx::Ptr<jxx::lang::String>& name,
                      const jxx::Ptr<jxx::lang::String>& actions);
        ~NetPermission() override = default;

    public:
        jxx::Ptr<jxx::lang::String> getName() const;
        jxx::Ptr<jxx::lang::String> getActions() const;
        jxx::lang::jbool implies(const jxx::Ptr<NetPermission>& p) const;

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        jxx::Ptr<jxx::lang::String> name_;
        jxx::Ptr<jxx::lang::String> actions_;
    };
}
