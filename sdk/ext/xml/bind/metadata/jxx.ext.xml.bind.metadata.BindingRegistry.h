#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang { class ClassAny; }
namespace jxx::ext::xml::bind::metadata { class BindingDescriptor; }

namespace jxx::ext::xml::bind::metadata {

class BindingRegistry final {
public:
    static void registerDescriptor(
        const ::jxx::Ptr<BindingDescriptor>& descriptor);

    static ::jxx::Ptr<BindingDescriptor> descriptorFor(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type);
};

} // namespace jxx::ext::xml::bind::metadata
