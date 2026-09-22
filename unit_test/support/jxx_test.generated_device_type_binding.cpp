#include "generated_device_type_binding.h"

#include "ext/xml/bind/jxx.ext.xml.bind.JAXBContext.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinConverters.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinFormatters.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.LexicalValidator.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ObjectFactory.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyReader.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyWriter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValidatingValueConverter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValidatingValueFormatter.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"

#include <cctype>

namespace jxx::test::generated::device {
namespace {

using namespace ::jxx::ext::xml::bind::metadata;

class DeviceType final : public ::jxx::lang::Object {
public:
    ::jxx::lang::jint id = 0;
    ::jxx::lang::jint config = 0;
    ::jxx::Ptr<::jxx::lang::String> hardwareFile;
    ::jxx::Ptr<::jxx::lang::String> securityFile;
};

enum class Property { ID, CONFIG, HARDWARE, SECURITY };

class DeviceFactory final
    : public ::jxx::lang::Object,
      public ObjectFactory {
public:
    ::jxx::Ptr<::jxx::lang::Object> create() override {
        return std::make_shared<DeviceType>();
    }
};

class DeviceWriter final
    : public ::jxx::lang::Object,
      public PropertyWriter {
public:
    explicit DeviceWriter(Property property) : property_(property) {}

    void write(
        const ::jxx::Ptr<::jxx::lang::Object>& target,
        const ::jxx::Ptr<::jxx::lang::Object>& value) override {
        const auto device = std::dynamic_pointer_cast<DeviceType>(target);
        switch (property_) {
            case Property::ID:
                device->id = std::dynamic_pointer_cast<::jxx::lang::Integer>(value)->intValue();
                break;
            case Property::CONFIG:
                device->config = std::dynamic_pointer_cast<::jxx::lang::Integer>(value)->intValue();
                break;
            case Property::HARDWARE:
                device->hardwareFile = std::dynamic_pointer_cast<::jxx::lang::String>(value);
                break;
            case Property::SECURITY:
                device->securityFile = std::dynamic_pointer_cast<::jxx::lang::String>(value);
                break;
        }
    }
private:
    Property property_;
};

class DeviceReader final
    : public ::jxx::lang::Object,
      public PropertyReader {
public:
    explicit DeviceReader(Property property) : property_(property) {}

    ::jxx::Ptr<::jxx::lang::Object> read(
        const ::jxx::Ptr<::jxx::lang::Object>& source) override {
        const auto device = std::dynamic_pointer_cast<DeviceType>(source);
        switch (property_) {
            case Property::ID:
                return ::jxx::CAST<::jxx::lang::Object>(
                    ::jxx::lang::Integer::valueOf(device->id));
            case Property::CONFIG:
                return ::jxx::CAST<::jxx::lang::Object>(
                    ::jxx::lang::Integer::valueOf(device->config));
            case Property::HARDWARE:
                return ::jxx::CAST<::jxx::lang::Object>(device->hardwareFile);
            case Property::SECURITY:
                return ::jxx::CAST<::jxx::lang::Object>(device->securityFile);
        }
        return nullptr;
    }
private:
    Property property_;
};

class IntegerRangeValidator final
    : public ::jxx::lang::Object,
      public LexicalValidator {
public:
    IntegerRangeValidator(::jxx::lang::jint minimum, ::jxx::lang::jint maximum)
        : minimum_(minimum), maximum_(maximum) {}

    void validate(
        const ::jxx::Ptr<::jxx::lang::String>& lexicalValue) override {
        const auto value = ::jxx::lang::Integer::parseInt(lexicalValue);
        if (value < minimum_ || value > maximum_) {
            throw ::jxx::lang::IllegalArgumentException(lexicalValue);
        }
    }
private:
    ::jxx::lang::jint minimum_;
    ::jxx::lang::jint maximum_;
};

class FilenameValidator final
    : public ::jxx::lang::Object,
      public LexicalValidator {
public:
    void validate(
        const ::jxx::Ptr<::jxx::lang::String>& lexicalValue) override {
        const auto text = lexicalValue->utf8();
        if (text.empty()) throw ::jxx::lang::IllegalArgumentException(lexicalValue);
        for (char current : text) {
            if (std::iscntrl(static_cast<unsigned char>(current)) ||
                current == '/' || current == '\\') {
                throw ::jxx::lang::IllegalArgumentException(lexicalValue);
            }
        }
    }
};

::jxx::Ptr<::jxx::lang::String> text(const char* value) {
    return ::jxx::NEW<::jxx::lang::String>(value);
}

::jxx::Ptr<PropertyBinding> inputBinding(
    const char* name,
    Property property,
    const ::jxx::Ptr<ValueConverter>& converter) {
    return ::jxx::NEW<PropertyBinding>(
        PropertyBinding::Kind::ATTRIBUTE,
        text(name), text(""), true, false, false,
        std::make_shared<DeviceWriter>(property), converter);
}

::jxx::Ptr<MarshallingBinding> outputBinding(
    const char* name,
    Property property,
    const ::jxx::Ptr<ValueFormatter>& formatter) {
    return ::jxx::NEW<MarshallingBinding>(
        PropertyBinding::Kind::ATTRIBUTE,
        text(name), text(""), true, false, false,
        std::make_shared<DeviceReader>(property), formatter);
}

::jxx::Ptr<LexicalValidator> deviceIdValidator() {
    return std::make_shared<IntegerRangeValidator>(0, 65535);
}

::jxx::Ptr<LexicalValidator> byteValidator() {
    return std::make_shared<IntegerRangeValidator>(0, 255);
}

::jxx::Ptr<LexicalValidator> filenameValidator() {
    return std::make_shared<FilenameValidator>();
}

} // namespace

::jxx::Ptr<::jxx::ext::xml::bind::JAXBContext> context() {
    using Descriptor = ::jxx::ext::xml::bind::metadata::BindingDescriptor;
    auto properties = ::jxx::NEW<Descriptor::PropertyArray>(4);
    (*properties)[0] = inputBinding(
        "id", Property::ID,
        ::jxx::NEW<ValidatingValueConverter>(
            ::jxx::NEW<IntegerConverter>(), deviceIdValidator()));
    (*properties)[1] = inputBinding(
        "config", Property::CONFIG,
        ::jxx::NEW<ValidatingValueConverter>(
            ::jxx::NEW<IntegerConverter>(), byteValidator()));
    (*properties)[2] = inputBinding(
        "hardware_scheme_filename", Property::HARDWARE,
        ::jxx::NEW<ValidatingValueConverter>(
            ::jxx::NEW<StringConverter>(), filenameValidator()));
    (*properties)[3] = inputBinding(
        "security_scheme_filename", Property::SECURITY,
        ::jxx::NEW<ValidatingValueConverter>(
            ::jxx::NEW<StringConverter>(), filenameValidator()));

    auto descriptors = ::jxx::NEW<
        ::jxx::ext::xml::bind::JAXBContext::DescriptorArray>(1);
    (*descriptors)[0] = ::jxx::NEW<Descriptor>(
        text("Scheduler.Generated.Slot.DeviceType"),
        text("device"), text(""),
        std::make_shared<DeviceFactory>(), properties);
    return ::jxx::ext::xml::bind::JAXBContext::newInstance(descriptors);
}

::jxx::Ptr<MarshallingDescriptor> marshallingDescriptor() {
    auto bindings = ::jxx::NEW<MarshallingDescriptor::BindingArray>(4);
    (*bindings)[0] = outputBinding(
        "id", Property::ID,
        ::jxx::NEW<ValidatingValueFormatter>(
            ::jxx::NEW<IntegerFormatter>(), deviceIdValidator()));
    (*bindings)[1] = outputBinding(
        "config", Property::CONFIG,
        ::jxx::NEW<ValidatingValueFormatter>(
            ::jxx::NEW<IntegerFormatter>(), byteValidator()));
    (*bindings)[2] = outputBinding(
        "hardware_scheme_filename", Property::HARDWARE,
        ::jxx::NEW<ValidatingValueFormatter>(
            ::jxx::NEW<StringFormatter>(), filenameValidator()));
    (*bindings)[3] = outputBinding(
        "security_scheme_filename", Property::SECURITY,
        ::jxx::NEW<ValidatingValueFormatter>(
            ::jxx::NEW<StringFormatter>(), filenameValidator()));
    return ::jxx::NEW<MarshallingDescriptor>(text("device"), text(""), bindings);
}

::jxx::Ptr<::jxx::lang::Object> newDevice(
    ::jxx::lang::jint idValue,
    ::jxx::lang::jint configValue,
    const ::jxx::Ptr<::jxx::lang::String>& hardwareFileValue,
    const ::jxx::Ptr<::jxx::lang::String>& securityFileValue) {
    const auto device = std::make_shared<DeviceType>();
    device->id = idValue;
    device->config = configValue;
    device->hardwareFile = hardwareFileValue;
    device->securityFile = securityFileValue;
    return device;
}

::jxx::lang::jint id(const ::jxx::Ptr<::jxx::lang::Object>& device) {
    return std::dynamic_pointer_cast<DeviceType>(device)->id;
}
::jxx::lang::jint config(const ::jxx::Ptr<::jxx::lang::Object>& device) {
    return std::dynamic_pointer_cast<DeviceType>(device)->config;
}
::jxx::Ptr<::jxx::lang::String> hardwareFile(
    const ::jxx::Ptr<::jxx::lang::Object>& device) {
    return std::dynamic_pointer_cast<DeviceType>(device)->hardwareFile;
}
::jxx::Ptr<::jxx::lang::String> securityFile(
    const ::jxx::Ptr<::jxx::lang::Object>& device) {
    return std::dynamic_pointer_cast<DeviceType>(device)->securityFile;
}

} // namespace jxx::test::generated::device
