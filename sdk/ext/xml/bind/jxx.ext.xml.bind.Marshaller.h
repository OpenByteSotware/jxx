#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::io { class Writer; }
namespace jxx::lang { class String; }
namespace jxx::ext::xml::bind { class JAXBElementI; }
namespace jxx::ext::xml::bind::metadata { class MarshallingDescriptor; }
namespace jxx::ext::xml::bind {
class Marshaller : public ::jxx::lang::InterfaceBase<Marshaller> {
public:
    ~Marshaller() override = default;
    virtual ::jxx::Ptr<::jxx::lang::String> marshal(
        const ::jxx::Ptr<::jxx::lang::Object>& object,
        const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor) = 0;
    virtual void marshal(
        const ::jxx::Ptr<::jxx::lang::Object>& object,
        const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor,
        const ::jxx::Ptr<::jxx::io::Writer>& writer) = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> marshalElement(
        const ::jxx::Ptr<JAXBElementI>& element,
        const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor) = 0;
    virtual void marshalElement(
        const ::jxx::Ptr<JAXBElementI>& element,
        const ::jxx::Ptr<metadata::MarshallingDescriptor>& descriptor,
        const ::jxx::Ptr<::jxx::io::Writer>& writer) = 0;
};
} // namespace jxx::ext::xml::bind
