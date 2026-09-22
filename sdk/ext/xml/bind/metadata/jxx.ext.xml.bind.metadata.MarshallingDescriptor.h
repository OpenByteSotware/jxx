#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingBinding.h"

namespace jxx::ext::xml::bind::metadata {

class MarshallingDescriptor final
    : public ::jxx::lang::ClassBase<
          MarshallingDescriptor,
          ::jxx::lang::Object> {
public:
    using BindingArray =
        ::jxx::lang::JxxArray<::jxx::Ptr<MarshallingBinding>, 1U>;

    MarshallingDescriptor(
        const ::jxx::Ptr<::jxx::lang::String>& rootName,
        const ::jxx::Ptr<::jxx::lang::String>& rootNamespace,
        const ::jxx::Ptr<BindingArray>& bindings);

    ::jxx::Ptr<::jxx::lang::String> rootName() const;
    ::jxx::Ptr<::jxx::lang::String> rootNamespace() const;
    ::jxx::Ptr<BindingArray> bindings() const;

private:
    ::jxx::Ptr<::jxx::lang::String> rootName_;
    ::jxx::Ptr<::jxx::lang::String> rootNamespace_;
    ::jxx::Ptr<BindingArray> bindings_;
};

} // namespace jxx::ext::xml::bind::metadata
