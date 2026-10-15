#include "io/jxx.io.ArrayCodecRegistry.h"
#include <mutex>
#include <unordered_map>
#include "lang/jxx.lang.IllegalStateException.h"

namespace jxx::io::serialization {
namespace {
struct WireKey { std::string component; std::uint32_t rank; bool operator==(const WireKey& x) const { return component == x.component && rank == x.rank; } };
struct WireHash { std::size_t operator()(const WireKey& x) const { return std::hash<std::string>{}(x.component) ^ (static_cast<std::size_t>(x.rank) << 1U); } };
std::mutex mutex;
std::unordered_map<std::type_index, ArrayCodec> byType;
std::unordered_map<WireKey, ArrayCodec, WireHash> byWire;
}
void ArrayCodecRegistry::registerCodec(const std::type_index& type, const ArrayCodec& codec) {
    std::lock_guard<std::mutex> lock(mutex);
    byType[type] = codec;
    byWire[WireKey{codec.componentBinaryName, codec.rank}] = codec;
}
const ArrayCodec* ArrayCodecRegistry::findByType(const std::type_index& type) {
    std::lock_guard<std::mutex> lock(mutex); const auto it = byType.find(type); return it == byType.end() ? nullptr : &it->second;
}
const ArrayCodec* ArrayCodecRegistry::findByWireType(const std::string& name, std::uint32_t rank) {
    std::lock_guard<std::mutex> lock(mutex); const auto it = byWire.find(WireKey{name, rank}); return it == byWire.end() ? nullptr : &it->second;
}
} // namespace jxx::io::serialization
