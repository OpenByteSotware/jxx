#include "lang/jxx.lang.Void.h"

#include <typeindex>

#include "lang/jxx.lang.Class.h"

namespace jxx::lang {

namespace {

::jxx::Ptr<ClassAny> registerVoidType() {
    ClassAny::Meta metadata;
    metadata.binaryName = "void";
    metadata.typeId = std::type_index(typeid(void));
    metadata.isPrimitive = true;
    metadata.modifiers = 0x0001 | 0x0010 | 0x0400;
    return ClassAny::registerClass(metadata);
}

} // namespace

const ::jxx::Ptr<ClassAny> Void::TYPE =
    registerVoidType();

} // namespace jxx::lang
