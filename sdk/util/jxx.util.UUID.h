#pragma once

#include <cstdint>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.CloneNotSupportedException.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::util {

class UUID final
    : public ::jxx::lang::ClassBase<UUID, ::jxx::lang::Object,
          ::jxx::lang::Comparable<UUID>, ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<UUID, JxxSuper,
        ::jxx::lang::Comparable<UUID>, ::jxx::io::SerializableI>;

    UUID(::jxx::lang::jlong mostSigBits,
         ::jxx::lang::jlong leastSigBits);

    static ::jxx::Ptr<UUID> randomUUID();
    static ::jxx::Ptr<UUID> nameUUIDFromBytes(
        const ::jxx::lang::ByteArray& name);
    static ::jxx::Ptr<UUID> fromString(
        const ::jxx::Ptr<::jxx::lang::String>& name);

    ::jxx::lang::jlong getMostSignificantBits() const;
    ::jxx::lang::jlong getLeastSignificantBits() const;
    ::jxx::lang::jint version() const;
    ::jxx::lang::jint variant() const;
    ::jxx::lang::jlong timestamp() const;
    ::jxx::lang::jint clockSequence() const;
    ::jxx::lang::jlong node() const;
    ::jxx::lang::jint compareTo(const ::jxx::Ptr<UUID>& value) const override;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& value) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) override;
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) override;
    void readObjectNoData() override;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;

private:
    ::jxx::lang::jlong mostSigBits_;
    ::jxx::lang::jlong leastSigBits_;
};

} // namespace jxx::util
