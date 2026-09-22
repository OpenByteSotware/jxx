#pragma once

#include "ext/xml/bind/jxx.ext.xml.bind.JAXBContext.h"
#include "ext/xml/bind/jxx.ext.xml.bind.Unmarshaller.h"

namespace jxx::ext::xml::bind::metadata { class BindingDescriptor; class PropertyBinding; }
namespace jxx::org::w3c::dom { class Element; class Node; }

namespace jxx::ext::xml::bind::internal {

class DomUnmarshaller final
    : public ::jxx::lang::ClassBase<
          DomUnmarshaller,
          ::jxx::lang::Object,
          ::jxx::ext::xml::bind::Unmarshaller> {
public:
    using DescriptorArray = JAXBContext::DescriptorArray;

    explicit DomUnmarshaller(
        const ::jxx::Ptr<DescriptorArray>& descriptors);

    ::jxx::Ptr<::jxx::lang::Object> unmarshal(
        const ::jxx::Ptr<::jxx::io::File>& file) override;
    ::jxx::Ptr<::jxx::lang::Object> unmarshal(
        const ::jxx::Ptr<::jxx::io::InputStream>& input) override;
    ::jxx::Ptr<::jxx::lang::Object> unmarshal(
        const ::jxx::Ptr<::jxx::io::Reader>& reader) override;
    ::jxx::Ptr<::jxx::lang::Object> unmarshal(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) override;

private:
    ::jxx::Ptr<metadata::BindingDescriptor> findRoot_(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Element>& element) const;
    ::jxx::Ptr<::jxx::lang::Object> readObject_(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Element>& element,
        const ::jxx::Ptr<metadata::BindingDescriptor>& descriptor) const;
    static ::jxx::Ptr<::jxx::lang::String> localName_(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node);
    static ::jxx::Ptr<::jxx::lang::String> namespace_(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node);

    ::jxx::Ptr<DescriptorArray> descriptors_;
};

} // namespace jxx::ext::xml::bind::internal
