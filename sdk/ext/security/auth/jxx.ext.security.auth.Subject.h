#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "security/jxx.security.Principal.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.Set.h"

namespace jxx::ext::security::auth {

class Subject final
    : public ::jxx::lang::ClassBase<Subject, ::jxx::lang::Object, ::jxx::io::SerializableI> {
public:
    Subject();
    Subject(::jxx::lang::jbool readOnly,
            const ::jxx::Ptr<::jxx::util::Set<::jxx::security::Principal>>& principals,
            const ::jxx::Ptr<::jxx::util::Set<::jxx::lang::Object>>& publicCredentials,
            const ::jxx::Ptr<::jxx::util::Set<::jxx::lang::Object>>& privateCredentials);

    void setReadOnly();
    ::jxx::lang::jbool isReadOnly() const;
    ::jxx::Ptr<::jxx::util::Set<::jxx::security::Principal>> getPrincipals();
    ::jxx::Ptr<::jxx::util::Set<::jxx::lang::Object>> getPublicCredentials();
    ::jxx::Ptr<::jxx::util::Set<::jxx::lang::Object>> getPrivateCredentials();
    ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
    void readObjectNoData() override;

private:
    ::jxx::lang::jbool readOnly_ = false;
    ::jxx::Ptr<::jxx::util::HashSet<::jxx::security::Principal>> principals_;
    ::jxx::Ptr<::jxx::util::HashSet<::jxx::lang::Object>> publicCredentials_;
    ::jxx::Ptr<::jxx::util::HashSet<::jxx::lang::Object>> privateCredentials_;
};

} // namespace jxx::ext::security::auth
