#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::ext::management::openmbean { class CompositeData; }

namespace jxx::lang::management {

class MemoryUsage final : public ::jxx::lang::ClassBase<MemoryUsage, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<MemoryUsage, JxxSuper>;

    MemoryUsage(::jxx::lang::jlong init, ::jxx::lang::jlong used,
        ::jxx::lang::jlong committed, ::jxx::lang::jlong max);
    ::jxx::lang::jlong getInit() const;
    ::jxx::lang::jlong getUsed() const;
    ::jxx::lang::jlong getCommitted() const;
    ::jxx::lang::jlong getMax() const;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
    static ::jxx::Ptr<MemoryUsage> from(
        const ::jxx::Ptr<::jxx::ext::management::openmbean::CompositeData>& data);
private:
    ::jxx::lang::jlong init_;
    ::jxx::lang::jlong used_;
    ::jxx::lang::jlong committed_;
    ::jxx::lang::jlong max_;
};

} // namespace jxx::lang::management
