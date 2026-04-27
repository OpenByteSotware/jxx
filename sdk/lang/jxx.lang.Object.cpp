//#include "jxx.lang.ClassNotFoundException.h"
#include "jxx.lang.ClassRegistration.h"
#include "jxx.lang.Class.h"
#include "jxx.lang.Object.h"
#include "lang/jxx.lang.Cast.h"

using namespace jxx::lang;

jxx::Ptr<ClassAny> Object::getClass() const {
    const TypeInfo* ti = TypeRegistry::instance().findByType(std::type_index(typeid(*this)));
    return std::make_shared<ClassAny>(ti);
}

// Virtual clone method
jxx::Ptr<jxx::lang::Object> Object::clone() const {
    // Check if this object is Cloneable
    if (JXX_CAST(Cloneable, shared_from_this()) == nullptr) {
        throw std::runtime_error("CloneNotSupportedException");
    }
    // If Cloneable, delegate to derived class's cloneImpl
    return cloneImpl();
}
