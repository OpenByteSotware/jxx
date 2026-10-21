#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <typeindex>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.JxxArray.h"

namespace jxx::io::serialization {

struct ArrayCodec final {
    using ObjectPtr = ::jxx::Ptr<::jxx::lang::Object>;
    std::string componentBinaryName;
    std::uint32_t rank = 0U;
    std::function<::jxx::lang::jint(const ObjectPtr&)> length;
    std::function<ObjectPtr(const ObjectPtr&, ::jxx::lang::jint)> get;
    std::function<void(const ObjectPtr&, ::jxx::lang::jint, const ObjectPtr&)> set;
    std::function<ObjectPtr(::jxx::lang::jint)> create;
};

class ArrayCodecRegistry final {
public:
    static void registerCodec(const std::type_index& type, const ArrayCodec& codec);
    static const ArrayCodec* findByType(const std::type_index& type);
    static const ArrayCodec* findByWireType(const std::string& componentBinaryName,
        std::uint32_t rank);
};

template <typename T, std::uint32_t Rank>
void registerClassReferenceArrayCodec() {
    static_assert(std::is_base_of_v<::jxx::lang::Object, T>,
        "T must be complete and derive from jxx::lang::Object");
    using Array = ::jxx::lang::JxxArray<::jxx::Ptr<T>, Rank>;
    using Row = ::jxx::lang::JxxArray<::jxx::Ptr<T>, Rank - 1U>;
    ArrayCodec codec;
    codec.componentBinaryName = T::Class()->getName()->utf8();
    codec.rank = Rank;
    codec.length = [](const ArrayCodec::ObjectPtr& value) { return ::jxx::CAST<Array>(value)->length; };
    codec.create = [](const ::jxx::lang::jint length) -> ArrayCodec::ObjectPtr {
        return ::jxx::CAST<::jxx::lang::Object>(::jxx::NEW<Array>(length));
    };
    if constexpr (Rank == 1U) {
        codec.get = [](const ArrayCodec::ObjectPtr& value, const ::jxx::lang::jint index) {
            return ::jxx::CAST<::jxx::lang::Object>((*::jxx::CAST<Array>(value))[index]);
        };
        codec.set = [](const ArrayCodec::ObjectPtr& value, const ::jxx::lang::jint index,
                       const ArrayCodec::ObjectPtr& element) {
            (*::jxx::CAST<Array>(value))[index] = element == nullptr ? nullptr : ::jxx::CAST<T>(element);
        };
    } else {
        codec.get = [](const ArrayCodec::ObjectPtr& value, const ::jxx::lang::jint index) {
            typename Array::RowPointer row = (*::jxx::CAST<Array>(value))[index];
            return ::jxx::CAST<::jxx::lang::Object>(row);
        };
        codec.set = [](const ArrayCodec::ObjectPtr& value, const ::jxx::lang::jint index,
                       const ArrayCodec::ObjectPtr& element) {
            (*::jxx::CAST<Array>(value))[index] = element == nullptr ? nullptr : ::jxx::CAST<Row>(element);
        };
    }
    ArrayCodecRegistry::registerCodec(std::type_index(typeid(Array)), codec);
}

template <typename T, std::uint32_t... Ranks>
void registerClassReferenceArrayCodecs() {
    (registerClassReferenceArrayCodec<T, Ranks>(), ...);
}

} // namespace jxx::io::serialization
