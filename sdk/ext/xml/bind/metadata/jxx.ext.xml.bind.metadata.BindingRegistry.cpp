#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingRegistry.h"

#include <mutex>
#include <string>
#include <unordered_map>

#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::bind::metadata {
namespace {

std::mutex& registryMutex() {
    static std::mutex value;
    return value;
}

std::unordered_map<std::string, ::jxx::Ptr<BindingDescriptor>>& registry() {
    static std::unordered_map<std::string, ::jxx::Ptr<BindingDescriptor>> value;
    return value;
}

std::string className(const ::jxx::Ptr<::jxx::lang::ClassAny>& type) {
    if (type == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    const auto name = type->getName();
    if (name == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return name->utf8();
}

} // namespace

void BindingRegistry::registerDescriptor(
    const ::jxx::Ptr<BindingDescriptor>& descriptor) {
    if (descriptor == nullptr || descriptor->declaredType() == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    std::lock_guard<std::mutex> lock(registryMutex());
    registry()[className(descriptor->declaredType())] = descriptor;
}

::jxx::Ptr<BindingDescriptor> BindingRegistry::descriptorFor(
    const ::jxx::Ptr<::jxx::lang::ClassAny>& type) {
    const auto name = className(type);
    std::lock_guard<std::mutex> lock(registryMutex());
    const auto found = registry().find(name);
    return found == registry().end() ? nullptr : found->second;
}

} // namespace jxx::ext::xml::bind::metadata
