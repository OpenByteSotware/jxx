#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::awt {

class Insets : public ::jxx::lang::ClassBase<
    Insets,
    ::jxx::lang::Object,
    ::jxx::lang::Cloneable,
    ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        Insets,
        JxxSuper,
        ::jxx::lang::Cloneable,
        ::jxx::io::SerializableI>;

    ::jxx::lang::jint top = 0;
    ::jxx::lang::jint left = 0;
    ::jxx::lang::jint bottom = 0;
    ::jxx::lang::jint right = 0;

    Insets() = default;
    Insets(::jxx::lang::jint top,
           ::jxx::lang::jint left,
           ::jxx::lang::jint bottom,
           ::jxx::lang::jint right);

    void set(::jxx::lang::jint top,
             ::jxx::lang::jint left,
             ::jxx::lang::jint bottom,
             ::jxx::lang::jint right);

    ::jxx::Ptr<::jxx::lang::Object> clone() const override;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override;
    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override;
    void readObjectNoData() override;
};

} // namespace jxx::awt
