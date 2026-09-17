#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "security/jxx.security.Guard.h"

namespace jxx::io { class ObjectInputStream; class ObjectOutputStream; }
namespace jxx::lang { class String; }
namespace jxx::security {

class Permission
    : public ::jxx::lang::ClassBase<
          Permission,
          ::jxx::lang::Object,
          Guard,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        Permission, JxxSuper, Guard, ::jxx::io::SerializableI>;

    explicit Permission(const ::jxx::Ptr<::jxx::lang::String>& name);
    ~Permission() override = default;

    void checkGuard(const ::jxx::Ptr<::jxx::lang::Object>& object) override;
    ::jxx::Ptr<::jxx::lang::String> getName() const;
    virtual ::jxx::lang::jbool implies(
        const ::jxx::Ptr<Permission>& permission) const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getActions() const = 0;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
    void readObjectNoData() override;

protected:
    ::jxx::Ptr<::jxx::lang::String> name_;
};

} // namespace jxx::security
