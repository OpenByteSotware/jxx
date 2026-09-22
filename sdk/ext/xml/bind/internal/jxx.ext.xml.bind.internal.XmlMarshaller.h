#pragma once
#include "ext/xml/bind/jxx.ext.xml.bind.Marshaller.h"
namespace jxx::ext::xml::bind::internal {
class XmlMarshaller final
    : public ::jxx::lang::ClassBase<XmlMarshaller, ::jxx::lang::Object,
          ::jxx::ext::xml::bind::Marshaller> {
public:
    ::jxx::Ptr<::jxx::lang::String> marshal(
        const ::jxx::Ptr<::jxx::lang::Object>& object,
        const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor) override;
    void marshal(
        const ::jxx::Ptr<::jxx::lang::Object>& object,
        const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor,
        const ::jxx::Ptr<::jxx::io::Writer>& writer) override;
private:
    static std::string writeObject_(
        const ::jxx::Ptr<::jxx::lang::Object>& object,
        const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor);
    static std::string escape_(const std::string& value, ::jxx::lang::jbool attribute);
};
} // namespace jxx::ext::xml::bind::internal
