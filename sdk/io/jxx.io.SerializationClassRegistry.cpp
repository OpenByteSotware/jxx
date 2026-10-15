#include "io/jxx.io.SerializationClassRegistry.h"

#include <mutex>
#include <unordered_map>

#include "io/jxx.io.InvalidClassException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io::serialization {
namespace {
std::mutex registryMutex;
std::unordered_map<std::string, SerializationClassMetadata> metadataByName;
}

void SerializationClassRegistry::registerClass(
    const std::string& binaryName,
    const SerializationClassMetadata& metadata)
{
    if (binaryName.empty() || metadata.serialVersionUID == 0 ||
        !metadata.deserializationFactory) {
        throw ::jxx::io::InvalidClassException(
            ::jxx::NEW<::jxx::lang::String>(binaryName),
            ::jxx::NEW<::jxx::lang::String>(
                "Invalid JXX serialization class metadata"));
    }
    std::lock_guard<std::mutex> lock(registryMutex);
    metadataByName[binaryName] = metadata;
}

SerializationClassMetadata SerializationClassRegistry::resolve(
    const std::string& binaryName)
{
    std::lock_guard<std::mutex> lock(registryMutex);
    const auto found = metadataByName.find(binaryName);
    if (found != metadataByName.end()) {
        return found->second;
    }
    SerializationClassMetadata fallback;
    fallback.serialVersionUID = 1;
    fallback.deserializationFactory = [binaryName] {
        return ::jxx::lang::ClassAny::forName(
            ::jxx::NEW<::jxx::lang::String>(binaryName))->newInstance();
    };
    return fallback;
}

} // namespace jxx::io::serialization
