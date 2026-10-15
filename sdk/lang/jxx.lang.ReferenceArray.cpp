#include "lang/jxx.lang.ReferenceArray.h"

#include <mutex>
#include <unordered_map>

#include "io/jxx.io.InvalidClassException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Class.h"

namespace jxx::lang {
std::string registeredClassBinaryName(const std::type_index& type) { return ClassAny::forType(type)->getName()->utf8(); }
namespace {
std::mutex registryMutex;
std::unordered_map<std::string, ReferenceArrayI::Factory> factories;
}

void ReferenceArrayRegistry::registerFactory(
    const std::string& componentBinaryName,
    const ReferenceArrayI::Factory& factory) {
    std::lock_guard<std::mutex> lock(registryMutex);
    factories.emplace(componentBinaryName, factory);
}

jxx::Ptr<Object> ReferenceArrayRegistry::create(
    const std::string& componentBinaryName,
    jint length) {
    ReferenceArrayI::Factory factory;
    {
        std::lock_guard<std::mutex> lock(registryMutex);
        const auto found = factories.find(componentBinaryName);
        if (found == factories.end()) {
            throw ::jxx::io::InvalidClassException(
                jxx::NEW<String>(componentBinaryName),
                jxx::NEW<String>("No typed reference-array factory is registered"));
        }
        factory = found->second;
    }
    return factory(length);
}

} // namespace jxx::lang
