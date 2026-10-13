#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ClassInitialization.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.AdaptedValueConverter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.AdaptedValueFormatter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"
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

namespace T = ::jxx::ext::xml::bind::metadata;

TEST(JxxStage56ClassInitialization, MetadataAndIdempotence)
{
    ::jxx::initialize();
    using namespace ::jxx::ext::xml::bind::metadata;
    EXPECT_NE(AdaptedValueConverter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(AdaptedValueFormatter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(BindingDescriptor::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ElementWrapperFactory::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(EnumValueConverter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(EnumValueFormatter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(EnumValueMap::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(LexicalValidator::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(LifecycleCallback::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(MarshallingBinding::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(MarshallingDescriptor::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(MixedContentReader::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(MixedContentWriter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ObjectFactory::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(PropertyBinding::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(PropertyReader::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(PropertyWriter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(RepeatedPropertyWriter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(RepeatedValueReader::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ValidatingValueConverter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ValidatingValueFormatter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ValueConverter::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ValueFormatter::JxxClassInfoMarker::Class(), nullptr);
    const auto before = AdaptedValueConverter::JxxClassInfoMarker::Class();
    EXPECT_NO_THROW(T::initializeClasses());
    EXPECT_NO_THROW(T::initializeClasses());
    EXPECT_EQ(before, AdaptedValueConverter::JxxClassInfoMarker::Class());
}
