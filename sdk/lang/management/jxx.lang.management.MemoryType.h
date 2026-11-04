#pragma once

#include "lang/jxx.lang.Enum.h"
#include "lang/jxx.lang.JxxArray.h"

namespace jxx::lang::management {
class MemoryType final : public ::jxx::lang::ClassBase<MemoryType, ::jxx::lang::Enum<MemoryType>> {
public:
    using JxxSuper = ::jxx::lang::Enum<MemoryType>;
    using Super = ::jxx::lang::ClassBase<MemoryType, JxxSuper>;
    static ::jxx::Ptr<MemoryType> HEAP;
    static ::jxx::Ptr<MemoryType> NON_HEAP;
    static ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<MemoryType>, 1U>> values();
    static ::jxx::Ptr<MemoryType> valueOf(
        const ::jxx::Ptr<::jxx::lang::String>& name);
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private:
    MemoryType(const char* name, ::jxx::lang::jint ordinal,
        const char* displayName);
    ::jxx::Ptr<::jxx::lang::String> displayName_;
};
} // namespace jxx::lang::management
