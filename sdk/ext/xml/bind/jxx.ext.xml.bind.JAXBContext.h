#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::ext::xml::bind::metadata { class BindingDescriptor; }
namespace jxx::ext::xml::bind { class Unmarshaller; class Marshaller; }

namespace jxx::ext::xml::bind {

class JAXBContext final
    : public ::jxx::lang::ClassBase<
          JAXBContext,
          ::jxx::lang::Object> {
public:
    using DescriptorArray =
        ::jxx::lang::JxxArray<
            ::jxx::Ptr<metadata::BindingDescriptor>,
            1U>;

    static ::jxx::Ptr<JAXBContext> newInstance(
        const ::jxx::Ptr<DescriptorArray>& descriptors);

    ::jxx::Ptr<Unmarshaller> createUnmarshaller();
    ::jxx::Ptr<Marshaller> createMarshaller();

    explicit JAXBContext(
        const ::jxx::Ptr<DescriptorArray>& descriptors);

    ::jxx::Ptr<DescriptorArray> descriptors_;
};

} // namespace jxx::ext::xml::bind
