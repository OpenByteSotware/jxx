#include "jxx.lang.ClassRegistration.h"
#include "jxx.lang.Class.h"
#include "jxx.lang.Object.h"

using namespace jxx::lang;

jxx::Ptr<ClassAny> Object::getClass() const {
    const TypeInfo* ti = TypeRegistry::instance().findByType(std::type_index(typeid(*this)));
    return std::make_shared<ClassAny>(ti);
}
