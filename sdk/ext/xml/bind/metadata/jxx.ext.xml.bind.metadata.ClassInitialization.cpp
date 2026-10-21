#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ClassInitialization.h"

#include <mutex>

#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.AdaptedValueConverter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.AdaptedValueFormatter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinConverters.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinFormatters.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ElementWrapperFactory.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.EnumValueConverter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.EnumValueFormatter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.EnumValueMap.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.LexicalValidator.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.LifecycleCallback.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingDescriptor.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MixedContentReader.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MixedContentWriter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ObjectFactory.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyReader.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyWriter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.RepeatedPropertyWriter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.RepeatedValueReader.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValidatingValueConverter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValidatingValueFormatter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueConverter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueFormatter.h"

namespace jxx::ext::xml::bind::metadata {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)AdaptedValueConverter::JxxClassInfoMarker::Class();
        (void)AdaptedValueFormatter::JxxClassInfoMarker::Class();
        (void)BindingDescriptor::JxxClassInfoMarker::Class();
        (void)StringConverter::Class();
        (void)IntegerConverter::Class();
        (void)LongConverter::Class();
        (void)DoubleConverter::Class();
        (void)BooleanConverter::Class();
        (void)StringFormatter::Class();
        (void)IntegerFormatter::Class();
        (void)LongFormatter::Class();
        (void)DoubleFormatter::Class();
        (void)BooleanFormatter::Class();
        (void)ElementWrapperFactory::JxxClassInfoMarker::Class();
        (void)EnumValueConverter::JxxClassInfoMarker::Class();
        (void)EnumValueFormatter::JxxClassInfoMarker::Class();
        (void)EnumValueMap::JxxClassInfoMarker::Class();
        (void)LexicalValidator::JxxClassInfoMarker::Class();
        (void)LifecycleCallback::JxxClassInfoMarker::Class();
        (void)MarshallingBinding::JxxClassInfoMarker::Class();
        (void)MarshallingDescriptor::JxxClassInfoMarker::Class();
        (void)MixedContentReader::JxxClassInfoMarker::Class();
        (void)MixedContentWriter::JxxClassInfoMarker::Class();
        (void)ObjectFactory::JxxClassInfoMarker::Class();
        (void)PropertyBinding::JxxClassInfoMarker::Class();
        (void)PropertyReader::JxxClassInfoMarker::Class();
        (void)PropertyWriter::JxxClassInfoMarker::Class();
        (void)RepeatedPropertyWriter::JxxClassInfoMarker::Class();
        (void)RepeatedValueReader::JxxClassInfoMarker::Class();
        (void)ValidatingValueConverter::JxxClassInfoMarker::Class();
        (void)ValidatingValueFormatter::JxxClassInfoMarker::Class();
        (void)ValueConverter::JxxClassInfoMarker::Class();
        (void)ValueFormatter::JxxClassInfoMarker::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::xml::bind::metadata
