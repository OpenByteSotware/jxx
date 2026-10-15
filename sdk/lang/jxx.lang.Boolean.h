#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class ClassAny;
class String;

class Boolean final
    : public ::jxx::lang::ClassBase<
          Boolean,
          Object,
          ::jxx::io::SerializableI,
          Comparable<Boolean>> {
public:
    using JxxSuper = Object;
    using Super = ::jxx::lang::ClassBase<
        Boolean,
        JxxSuper,
        ::jxx::io::SerializableI,
        Comparable<Boolean>>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<
        Boolean,
        JxxSuper,
        ::jxx::io::SerializableI,
        Comparable<Boolean>>;

    static ::jxx::Ptr<ClassAny> Class();

    static ::jxx::Ptr<Boolean> TRUE_;
    static ::jxx::Ptr<Boolean> FALSE_;
    static const ::jxx::Ptr<ClassAny> TYPE;

    explicit Boolean(::jxx::lang::jbool value);
    explicit Boolean(const ::jxx::Ptr<String>& value);
    ~Boolean() override = default;

    static ::jxx::Ptr<Boolean> valueOf(::jxx::lang::jbool value);
    static ::jxx::Ptr<Boolean> valueOf(const ::jxx::Ptr<String>& value);
    static ::jxx::lang::jbool parseBoolean(
        const ::jxx::Ptr<String>& value);
    static ::jxx::lang::jbool getBoolean(
        const ::jxx::Ptr<String>& name);

    ::jxx::lang::jbool booleanValue() const;

    ::jxx::Ptr<String> toString() const override;
    static ::jxx::Ptr<String> toString(::jxx::lang::jbool value);

    ::jxx::lang::jint hashCode() const override;
    static ::jxx::lang::jint hashCode(::jxx::lang::jbool value);

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<Object>& object) const override;

    ::jxx::lang::jint compareTo(
        const ::jxx::Ptr<Boolean>& other) const override;
    static ::jxx::lang::jint compare(
        ::jxx::lang::jbool left,
        ::jxx::lang::jbool right);

    static ::jxx::lang::jbool logicalAnd(
        ::jxx::lang::jbool left,
        ::jxx::lang::jbool right) noexcept;
    static ::jxx::lang::jbool logicalOr(
        ::jxx::lang::jbool left,
        ::jxx::lang::jbool right) noexcept;
    static ::jxx::lang::jbool logicalXor(
        ::jxx::lang::jbool left,
        ::jxx::lang::jbool right) noexcept;

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
    void readObjectNoData() override;

private:
    ::jxx::lang::jbool value_;
};

} // namespace jxx::lang
