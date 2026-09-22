#pragma once

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace com::google::gson::reflect {

class TypeToken final
    : public ::jxx::lang::ClassBase<TypeToken, ::jxx::lang::Object> {
public:
    explicit TypeToken(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& rawType);

    static ::jxx::Ptr<TypeToken> get(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type);

    ::jxx::Ptr<::jxx::lang::ClassAny> getRawType() const;
    ::jxx::Ptr<::jxx::lang::ClassAny> getType() const;
    ::jxx::lang::jbool isAssignableFrom(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type) const;
    ::jxx::lang::jbool isAssignableFrom(
        const ::jxx::Ptr<TypeToken>& type) const;

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

private:
    ::jxx::Ptr<::jxx::lang::ClassAny> rawType_;
};

} // namespace com::google::gson::reflect
