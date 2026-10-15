#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io::serialization {

struct SerializationClassMetadata final {
    using Factory = std::function<::jxx::Ptr<::jxx::lang::Object>()>;
    ::jxx::lang::jlong serialVersionUID = 1;
    Factory deserializationFactory;
};

class SerializationClassRegistry final {
public:
    static void registerClass(const std::string& binaryName,
        const SerializationClassMetadata& metadata);
    static SerializationClassMetadata resolve(const std::string& binaryName);
};

template <typename T>
void registerSerializableClass(
    ::jxx::lang::jlong serialVersionUID = 1)
{
    static_assert(std::is_base_of_v<::jxx::lang::Object, T>,
        "T must be complete and derive from jxx::lang::Object");
    static_assert(std::is_default_constructible_v<T>,
        "T must be default constructible or use the factory overload");
    SerializationClassMetadata metadata;
    metadata.serialVersionUID = serialVersionUID;
    metadata.deserializationFactory = [] {
        return ::jxx::CAST<::jxx::lang::Object>(::jxx::NEW<T>());
    };
    SerializationClassRegistry::registerClass(
        T::Class()->getName()->utf8(), metadata);
}

template <typename T, typename Factory>
void registerSerializableClass(
    ::jxx::lang::jlong serialVersionUID,
    Factory&& factory)
{
    static_assert(std::is_base_of_v<::jxx::lang::Object, T>,
        "T must be complete and derive from jxx::lang::Object");
    SerializationClassMetadata metadata;
    metadata.serialVersionUID = serialVersionUID;
    metadata.deserializationFactory =
        [factory = std::forward<Factory>(factory)]() mutable {
            return ::jxx::CAST<::jxx::lang::Object>(factory());
        };
    SerializationClassRegistry::registerClass(
        T::Class()->getName()->utf8(), metadata);
}

} // namespace jxx::io::serialization
