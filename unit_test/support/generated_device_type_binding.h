#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind { class JAXBContext; }
namespace jxx::ext::xml::bind::metadata { class MarshallingDescriptor; }

namespace jxx::test::generated::device {

::jxx::Ptr<::jxx::ext::xml::bind::JAXBContext> context();
::jxx::Ptr<::jxx::ext::xml::bind::metadata::MarshallingDescriptor>
marshallingDescriptor();

::jxx::Ptr<::jxx::lang::Object> newDevice(
    ::jxx::lang::jint id,
    ::jxx::lang::jint config,
    const ::jxx::Ptr<::jxx::lang::String>& hardwareFile,
    const ::jxx::Ptr<::jxx::lang::String>& securityFile);

::jxx::lang::jint id(const ::jxx::Ptr<::jxx::lang::Object>& device);
::jxx::lang::jint config(const ::jxx::Ptr<::jxx::lang::Object>& device);
::jxx::Ptr<::jxx::lang::String> hardwareFile(
    const ::jxx::Ptr<::jxx::lang::Object>& device);
::jxx::Ptr<::jxx::lang::String> securityFile(
    const ::jxx::Ptr<::jxx::lang::Object>& device);

} // namespace jxx::test::generated::device
