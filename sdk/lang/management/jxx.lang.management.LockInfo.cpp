#include "lang/management/jxx.lang.management.LockInfo.h"

#include <iomanip>
#include <sstream>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::lang::management {
LockInfo::LockInfo(const ::jxx::Ptr<::jxx::lang::String>& className,
    ::jxx::lang::jint identityHashCode)
    : className_(className), identityHashCode_(identityHashCode) {
    if (className_ == nullptr) throw ::jxx::lang::NullPointerException("className");
}
::jxx::Ptr<::jxx::lang::String> LockInfo::getClassName() const { return className_; }
::jxx::lang::jint LockInfo::getIdentityHashCode() const { return identityHashCode_; }
::jxx::Ptr<LockInfo> LockInfo::from(
    const ::jxx::Ptr<::jxx::ext::management::openmbean::CompositeData>& data) {
    if (data == nullptr) return nullptr;
    throw ::jxx::lang::UnsupportedOperationException(
        "CompositeData requires javax.management.openmbean support");
}
::jxx::Ptr<::jxx::lang::String> LockInfo::toString() const {
    std::ostringstream out;
    out << className_->utf8() << '@' << std::hex
        << static_cast<std::uint32_t>(identityHashCode_);
    return ::jxx::NEW<::jxx::lang::String>(out.str());
}
} // namespace jxx::lang::management
