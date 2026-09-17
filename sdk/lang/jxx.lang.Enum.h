#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.CloneNotSupportedException.h"
#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

template <typename E>
class Enum
    : public ClassBase<Enum<E>, Object, Comparable<E>, jxx::io::SerializableI> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<Enum<E>, JxxSuper, Comparable<E>, jxx::io::SerializableI>;

    jxx::Ptr<String> name() const { return name_; }
    jint ordinal() const noexcept { return ordinal_; }

    jxx::Ptr<String> toString() const override { return name_; }

    jbool equals(const jxx::Ptr<Object>& other) const override {
        return this == other.get();
    }

    jint hashCode() const override { return Object::hashCode(); }

    jint compareTo(const jxx::Ptr<E>& other) const override {
        if (other == nullptr) throw NullPointerException();
        const auto* value = dynamic_cast<const Enum<E>*>(other.get());
        if (value == nullptr) throw ClassCastException();
        return ordinal_ - value->ordinal_;
    }

    jxx::Ptr<ClassAny> getDeclaringClass() const { return E::Class(); }

    void writeObject(const jxx::Ptr<jxx::io::ObjectOutputStream>& out) override { (void)out; }
    void readObject(const jxx::Ptr<jxx::io::ObjectInputStream>& in) override { (void)in; }
    void readObjectNoData() override {}

    static jxx::Ptr<E> valueOf(
        const jxx::Ptr<ClassAny>& enumType,
        const jxx::Ptr<String>& name) {
        if (enumType == nullptr || name == nullptr) {
            throw NullPointerException();
        }
        if (enumType.get() != E::Class().get()) {
            throw IllegalArgumentException();
        }
        const auto constants = E::values();
        if (constants != nullptr) {
            for (std::uint32_t index = 0; index < constants->length; ++index) {
                const auto& constant = (*constants)[index];
                if (constant != nullptr && constant->name()->equals(name)) {
                    return constant;
                }
            }
        }
        throw IllegalArgumentException(name);
    }

protected:
    Enum(const jxx::Ptr<String>& name, jint ordinal)
        : Super(), name_(name), ordinal_(ordinal) {
        if (name_ == nullptr) throw NullPointerException();
    }

    jxx::Ptr<Object> cloneImpl() const override {
        throw CloneNotSupportedException();
    }

private:
    jxx::Ptr<String> name_;
    jint ordinal_;
};

} // namespace jxx::lang
