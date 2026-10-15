#pragma once

#include <functional>
#include <string>
#include <typeindex>

#include "lang/jxx_types.h"

namespace jxx::lang {
class Object;
std::string registeredClassBinaryName(const std::type_index& type);

class ReferenceArrayI {
public:
    using Factory = std::function<jxx::Ptr<Object>(jint)>;

    virtual ~ReferenceArrayI() = default;
    virtual std::string componentBinaryName() const = 0;
    virtual jint referenceLength() const noexcept = 0;
    virtual jxx::Ptr<Object> referenceElement(jint index) const = 0;
    virtual void setReferenceElement(jint index, const jxx::Ptr<Object>& value) = 0;
    virtual Factory referenceArrayFactory() const = 0;
};

class ReferenceArrayRegistry final {
public:
    static void registerFactory(const std::string& componentBinaryName,
        const ReferenceArrayI::Factory& factory);
    static jxx::Ptr<Object> create(const std::string& componentBinaryName,
        jint length);
};

} // namespace jxx::lang
