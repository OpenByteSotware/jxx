#include "com/google/gson/jxx.com.google.gson.GsonBuilder.h"
#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace com::google::gson {
namespace {
template <typename T>
::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<T>, 1U>> append(
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<T>, 1U>>& values,
    const ::jxx::Ptr<T>& value) {
    const ::jxx::lang::jint oldLength = values == nullptr ? 0 : values->length;
    auto result = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<T>, 1U>>(oldLength + 1);
    for (::jxx::lang::jint index = 0; index < oldLength; ++index) (*result)[index] = (*values)[index];
    (*result)[oldLength] = value;
    return result;
}
}

GsonBuilder::GsonBuilder(const ::jxx::Ptr<Gson>& gson) {
    if (gson == nullptr) throw ::jxx::lang::NullPointerException();
    serializeNulls_ = gson->serializeNulls();
    htmlSafe_ = gson->htmlSafe();
    prettyPrinting_ = gson->prettyPrinting();
    lenient_ = gson->lenient();
    fieldNamingStrategy_ = gson->fieldNamingStrategy();
    requireExpose_ = gson->requireExpose();
    version_ = gson->version();
    longSerializationPolicy_ = gson->longSerializationPolicy();
    objectToNumberStrategy_ = gson->objectToNumberStrategy();
    numberToNumberStrategy_ = gson->numberToNumberStrategy();
    dateFormatPattern_ = gson->dateFormatPattern();
    dateStyle_ = gson->dateStyle();
    serializeInnerClasses_ = gson->serializeInnerClasses();
    complexMapKeySerialization_ = gson->complexMapKeySerialization();
}

::jxx::Ptr<GsonBuilder> GsonBuilder::serializeNulls() { serializeNulls_ = true; return ::jxx::CAST<GsonBuilder>(this->thisPtr()); }
::jxx::Ptr<GsonBuilder> GsonBuilder::disableHtmlEscaping() { htmlSafe_ = false; return ::jxx::CAST<GsonBuilder>(this->thisPtr()); }
::jxx::Ptr<GsonBuilder> GsonBuilder::setPrettyPrinting() { prettyPrinting_ = true; return ::jxx::CAST<GsonBuilder>(this->thisPtr()); }
::jxx::Ptr<GsonBuilder> GsonBuilder::setLenient() { lenient_ = true; return ::jxx::CAST<GsonBuilder>(this->thisPtr()); }

::jxx::Ptr<GsonBuilder> GsonBuilder::setFieldNamingPolicy(FieldNamingPolicy policy) {
    fieldNamingStrategy_ = FieldNamingPolicies::strategy(policy);
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::setFieldNamingStrategy(
    const ::jxx::Ptr<FieldNamingStrategy>& strategy) {
    if (strategy == nullptr) throw ::jxx::lang::NullPointerException();
    fieldNamingStrategy_ = strategy;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::registerTypeAdapter(
    const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
    const ::jxx::Ptr<TypeAdapter>& adapter) {
    if (type == nullptr || adapter == nullptr) throw ::jxx::lang::NullPointerException();
    adapterTypes_ = append(adapterTypes_, type);
    adapters_ = append(adapters_, adapter);
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::registerInstanceCreator(
    const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
    const ::jxx::Ptr<InstanceCreator>& creator) {
    if (type == nullptr || creator == nullptr) throw ::jxx::lang::NullPointerException();
    creatorTypes_ = append(creatorTypes_, type);
    creators_ = append(creators_, creator);
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::registerTypeAdapterFactory(
    const ::jxx::Ptr<TypeAdapterFactory>& factory) {
    if (factory == nullptr) throw ::jxx::lang::NullPointerException();
    factories_ = append(factories_, factory);
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}


::jxx::Ptr<GsonBuilder> GsonBuilder::addSerializationExclusionStrategy(
    const ::jxx::Ptr<ExclusionStrategy>& strategy) {
    if (strategy == nullptr) throw ::jxx::lang::NullPointerException();
    serializationStrategies_ = append(serializationStrategies_, strategy);
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::addDeserializationExclusionStrategy(
    const ::jxx::Ptr<ExclusionStrategy>& strategy) {
    if (strategy == nullptr) throw ::jxx::lang::NullPointerException();
    deserializationStrategies_ = append(deserializationStrategies_, strategy);
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::setExclusionStrategies(
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<ExclusionStrategy>, 1U>>& strategies) {
    if (strategies == nullptr) throw ::jxx::lang::NullPointerException();
    serializationStrategies_ = strategies;
    deserializationStrategies_ = strategies;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::excludeFieldsWithoutExposeAnnotation() {
    requireExpose_ = true;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::setVersion(::jxx::lang::jdouble version) {
    if (version < 0.0) throw ::jxx::lang::IllegalArgumentException();
    version_ = version;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::generateNonExecutableJson() {
    nonExecutableJson_ = true;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::serializeSpecialFloatingPointValues() {
    specialFloatingPointValues_ = true;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::setLongSerializationPolicy(
    LongSerializationPolicy policy) {
    longSerializationPolicy_ = policy;
    return ::jxx::CAST<GsonBuilder>(this->thisPtr());
}

::jxx::Ptr<GsonBuilder> GsonBuilder::setObjectToNumberStrategy(const ::jxx::Ptr<ToNumberStrategy>& strategy){if(strategy==nullptr)throw ::jxx::lang::NullPointerException();objectToNumberStrategy_=strategy;return ::jxx::CAST<GsonBuilder>(this->thisPtr());}
::jxx::Ptr<GsonBuilder> GsonBuilder::setNumberToNumberStrategy(const ::jxx::Ptr<ToNumberStrategy>& strategy){if(strategy==nullptr)throw ::jxx::lang::NullPointerException();numberToNumberStrategy_=strategy;return ::jxx::CAST<GsonBuilder>(this->thisPtr());}
::jxx::Ptr<GsonBuilder> GsonBuilder::setDateFormat(const ::jxx::Ptr<::jxx::lang::String>& pattern){if(pattern==nullptr)throw ::jxx::lang::NullPointerException();dateFormatPattern_=pattern;dateStyle_=-1;return ::jxx::CAST<GsonBuilder>(this->thisPtr());}
::jxx::Ptr<GsonBuilder> GsonBuilder::setDateFormat(::jxx::lang::jint style){dateStyle_=style;dateFormatPattern_=nullptr;return ::jxx::CAST<GsonBuilder>(this->thisPtr());}
::jxx::Ptr<GsonBuilder> GsonBuilder::disableInnerClassSerialization(){serializeInnerClasses_=false;return ::jxx::CAST<GsonBuilder>(this->thisPtr());}
::jxx::Ptr<GsonBuilder> GsonBuilder::enableComplexMapKeySerialization(){complexMapKeySerialization_=true;return ::jxx::CAST<GsonBuilder>(this->thisPtr());}

::jxx::Ptr<GsonBuilder> GsonBuilder::registerTypeSerializer(const ::jxx::Ptr<::jxx::lang::ClassAny>& type,const ::jxx::Ptr<JsonSerializer>& serializer){if(type==nullptr||serializer==nullptr)throw ::jxx::lang::NullPointerException();serializerTypes_=append(serializerTypes_,type);serializers_=append(serializers_,serializer);return ::jxx::CAST<GsonBuilder>(this->thisPtr());}
::jxx::Ptr<GsonBuilder> GsonBuilder::registerTypeDeserializer(const ::jxx::Ptr<::jxx::lang::ClassAny>& type,const ::jxx::Ptr<JsonDeserializer>& deserializer){if(type==nullptr||deserializer==nullptr)throw ::jxx::lang::NullPointerException();deserializerTypes_=append(deserializerTypes_,type);deserializers_=append(deserializers_,deserializer);return ::jxx::CAST<GsonBuilder>(this->thisPtr());}

::jxx::Ptr<GsonBuilder> GsonBuilder::registerTypeHierarchyAdapter(const ::jxx::Ptr<::jxx::lang::ClassAny>& baseType,const ::jxx::Ptr<TypeAdapter>& adapter){if(baseType==nullptr||adapter==nullptr)throw ::jxx::lang::NullPointerException();hierarchyAdapterTypes_=append(hierarchyAdapterTypes_,baseType);hierarchyAdapters_=append(hierarchyAdapters_,adapter);return ::jxx::CAST<GsonBuilder>(this->thisPtr());}

::jxx::Ptr<Gson> GsonBuilder::create() const {
    return ::jxx::NEW<Gson>(serializeNulls_, htmlSafe_, prettyPrinting_, lenient_,
        fieldNamingStrategy_, adapterTypes_, adapters_, creatorTypes_, creators_, factories_,
        serializationStrategies_, deserializationStrategies_, requireExpose_, version_,
        nonExecutableJson_, specialFloatingPointValues_, longSerializationPolicy_,
        objectToNumberStrategy_, numberToNumberStrategy_, dateFormatPattern_, dateStyle_,
        serializeInnerClasses_, complexMapKeySerialization_, serializerTypes_, serializers_, deserializerTypes_, deserializers_);
}

} // namespace com::google::gson
