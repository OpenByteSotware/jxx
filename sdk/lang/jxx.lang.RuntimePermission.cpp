#include "lang/jxx.lang.RuntimePermission.h"
#include "lang/jxx.lang.Class.h"

namespace jxx::lang {
::jxx::Ptr<ClassAny> RuntimePermission::Class() { return JxxClassInfoMarker::Class(); }
RuntimePermission::RuntimePermission(const ::jxx::Ptr<String>& name) : Super(name) {}
RuntimePermission::RuntimePermission(const ::jxx::Ptr<String>& name,
    const ::jxx::Ptr<String>& actions) : Super(name, actions) {}
} // namespace jxx::lang
