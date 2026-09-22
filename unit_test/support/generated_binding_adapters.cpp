#include "support/jxx_test.generated_binding_adapters.h"

#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinConverters.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyWriter.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"

namespace jxx::test::generated {
namespace {

class PortModel final : public ::jxx::lang::Object {
public:
    explicit PortModel(::jxx::lang::jint value = 0)
        : port(value) {}

    ::jxx::lang::jint port;
};

class GeneratedPortJsonAdapter final
    : public ::jxx::lang::Object,
      public ::com::google::gson::TypeAdapter {
public:
    ::jxx::Ptr<::jxx::lang::Object> fromJsonTree(
        const ::jxx::Ptr<::com::google::gson::JsonElement>& element)
        override {
        const auto object = element->getAsJsonObject();
        const auto portElement = object->get(
            ::jxx::NEW<::jxx::lang::String>("port"));
        return std::make_shared<PortModel>(
            portElement->getAsInt());
    }

    ::jxx::Ptr<::com::google::gson::JsonElement> toJsonTree(
        const ::jxx::Ptr<::jxx::lang::Object>& value)
        override {
        const auto model =
            std::dynamic_pointer_cast<PortModel>(value);
        const auto object =
            ::jxx::NEW<::com::google::gson::JsonObject>();
        object->add(
            ::jxx::NEW<::jxx::lang::String>("port"),
            ::jxx::CAST<::com::google::gson::JsonElement>(
                ::jxx::NEW<::com::google::gson::JsonPrimitive>(
                    static_cast<::jxx::lang::jlong>(model->port))));
        return ::jxx::CAST<::com::google::gson::JsonElement>(object);
    }
};

class GeneratedPortPropertyWriter final
    : public ::jxx::lang::Object,
      public ::jxx::ext::xml::bind::metadata::PropertyWriter {
public:
    void write(
        const ::jxx::Ptr<::jxx::lang::Object>& target,
        const ::jxx::Ptr<::jxx::lang::Object>& value)
        override {
        const auto model =
            std::dynamic_pointer_cast<PortModel>(target);
        const auto integer =
            std::dynamic_pointer_cast<::jxx::lang::Integer>(value);
        model->port = integer->intValue();
    }
};

} // namespace

::jxx::Ptr<::com::google::gson::TypeAdapter>
portJsonAdapter() {
    return std::make_shared<GeneratedPortJsonAdapter>();
}

::jxx::Ptr<::jxx::lang::Object>
newPortModel(::jxx::lang::jint port) {
    return std::make_shared<PortModel>(port);
}

::jxx::lang::jint
portFromModel(
    const ::jxx::Ptr<::jxx::lang::Object>& model) {
    return std::dynamic_pointer_cast<PortModel>(model)->port;
}

::jxx::Ptr<
    ::jxx::ext::xml::bind::metadata::PropertyBinding>
portXmlBinding() {
    using namespace ::jxx::ext::xml::bind::metadata;
    return ::jxx::NEW<PropertyBinding>(
        PropertyBinding::Kind::ELEMENT,
        ::jxx::NEW<::jxx::lang::String>("port"),
        ::jxx::NEW<::jxx::lang::String>(""),
        true,
        false,
        false,
        std::make_shared<GeneratedPortPropertyWriter>(),
        ::jxx::CAST<ValueConverter>(
            ::jxx::NEW<IntegerConverter>()));
}

} // namespace jxx::test::generated
