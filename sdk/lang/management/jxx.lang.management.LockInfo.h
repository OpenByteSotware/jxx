#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::ext::management::openmbean { class CompositeData; }

namespace jxx::lang::management {
class LockInfo : public ::jxx::lang::ClassBase<LockInfo, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<LockInfo, JxxSuper>;
    LockInfo(const ::jxx::Ptr<::jxx::lang::String>& className,
        ::jxx::lang::jint identityHashCode);
    ::jxx::Ptr<::jxx::lang::String> getClassName() const;
    ::jxx::lang::jint getIdentityHashCode() const;
    static ::jxx::Ptr<LockInfo> from(
        const ::jxx::Ptr<::jxx::ext::management::openmbean::CompositeData>& data);
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private:
    ::jxx::Ptr<::jxx::lang::String> className_;
    ::jxx::lang::jint identityHashCode_;
};
} // namespace jxx::lang::management
