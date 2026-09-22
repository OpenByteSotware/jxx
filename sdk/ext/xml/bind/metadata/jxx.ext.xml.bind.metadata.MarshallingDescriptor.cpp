#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingDescriptor.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind::metadata {

MarshallingDescriptor::MarshallingDescriptor(
    const ::jxx::Ptr<::jxx::lang::String>& rootName,
    const ::jxx::Ptr<::jxx::lang::String>& rootNamespace,
    const ::jxx::Ptr<BindingArray>& bindings)
    : rootName_(rootName), rootNamespace_(rootNamespace), bindings_(bindings) {
    if (rootName_ == nullptr || rootNamespace_ == nullptr || bindings_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::String> MarshallingDescriptor::rootName() const { return rootName_; }
::jxx::Ptr<::jxx::lang::String> MarshallingDescriptor::rootNamespace() const { return rootNamespace_; }
::jxx::Ptr<MarshallingDescriptor::BindingArray> MarshallingDescriptor::bindings() const { return bindings_; }

} // namespace jxx::ext::xml::bind::metadata
