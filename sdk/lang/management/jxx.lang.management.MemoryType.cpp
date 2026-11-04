#include "lang/management/jxx.lang.management.MemoryType.h"

namespace jxx::lang::management {
MemoryType::MemoryType(const char* name, ::jxx::lang::jint ordinal,
    const char* displayName)
    : Super(::jxx::NEW<::jxx::lang::String>(name), ordinal),
      displayName_(::jxx::NEW<::jxx::lang::String>(displayName)) {}
::jxx::Ptr<MemoryType> MemoryType::HEAP(
    new MemoryType("HEAP", 0, "Heap memory"));
::jxx::Ptr<MemoryType> MemoryType::NON_HEAP(
    new MemoryType("NON_HEAP", 1, "Non-heap memory"));
::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<MemoryType>, 1U>>
MemoryType::values() {
    auto result = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<MemoryType>, 1U>>(2);
    (*result)[0] = HEAP;
    (*result)[1] = NON_HEAP;
    return result;
}
::jxx::Ptr<MemoryType> MemoryType::valueOf(
    const ::jxx::Ptr<::jxx::lang::String>& name) {
    return JxxSuper::valueOf(MemoryType::Class(), name);
}
::jxx::Ptr<::jxx::lang::String> MemoryType::toString() const {
    return displayName_;
}
} // namespace jxx::lang::management
