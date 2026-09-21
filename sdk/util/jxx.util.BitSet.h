#pragma once

#include <cstdint>
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::util {

class BitSet final
    : public ::jxx::lang::ClassBase<BitSet, ::jxx::lang::Object,
          ::jxx::lang::Cloneable, ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<BitSet, JxxSuper,
        ::jxx::lang::Cloneable, ::jxx::io::SerializableI>;

    BitSet();
    explicit BitSet(::jxx::lang::jint numberOfBits);

    void flip(::jxx::lang::jint bitIndex);
    void flip(::jxx::lang::jint fromIndex, ::jxx::lang::jint toIndex);
    void set(::jxx::lang::jint bitIndex);
    void set(::jxx::lang::jint bitIndex, ::jxx::lang::jbool value);
    void set(::jxx::lang::jint fromIndex, ::jxx::lang::jint toIndex);
    void set(::jxx::lang::jint fromIndex, ::jxx::lang::jint toIndex,
        ::jxx::lang::jbool value);
    void clear(::jxx::lang::jint bitIndex);
    void clear(::jxx::lang::jint fromIndex, ::jxx::lang::jint toIndex);
    void clear();
    ::jxx::lang::jbool get(::jxx::lang::jint bitIndex) const;
    ::jxx::Ptr<BitSet> get(::jxx::lang::jint fromIndex,
        ::jxx::lang::jint toIndex) const;
    ::jxx::lang::jint nextSetBit(::jxx::lang::jint fromIndex) const;
    ::jxx::lang::jint nextClearBit(::jxx::lang::jint fromIndex) const;
    ::jxx::lang::jint previousSetBit(::jxx::lang::jint fromIndex) const;
    ::jxx::lang::jint previousClearBit(::jxx::lang::jint fromIndex) const;
    ::jxx::lang::jint length() const;
    ::jxx::lang::jbool isEmpty() const;
    ::jxx::lang::jbool intersects(const ::jxx::Ptr<BitSet>& value) const;
    ::jxx::lang::jint cardinality() const;
    void andSet(const ::jxx::Ptr<BitSet>& value);
    void orSet(const ::jxx::Ptr<BitSet>& value);
    void xorSet(const ::jxx::Ptr<BitSet>& value);
    void andNot(const ::jxx::Ptr<BitSet>& value);
    ::jxx::lang::jint size() const;
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
    static void checkIndex_(::jxx::lang::jint index);
    static void checkRange_(::jxx::lang::jint fromIndex,
        ::jxx::lang::jint toIndex);
    void ensure_(::jxx::lang::jint index);
    void trim_();
    std::vector<std::uint64_t> words_;
};

} // namespace jxx::util
