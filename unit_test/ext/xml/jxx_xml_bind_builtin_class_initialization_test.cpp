#include <gtest/gtest.h>

#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinConverters.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinFormatters.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ClassInitialization.h"
#include "jxx.initialize.h"

namespace
{
    template <typename T>
    void expectClassMetadata()
    {
        const auto type = T::Class();
        ASSERT_NE(type, nullptr);
        ASSERT_NE(type->getName(), nullptr);
        EXPECT_FALSE(type->getName()->utf8().empty());
    }

    TEST(XmlBindMetadataClassInitialization, RegistersBuiltinConvertersAndFormatters)
    {
        jxx::initialize();
        using namespace ::jxx::ext::xml::bind::metadata;
        //initializeClasses();
        expectClassMetadata<StringConverter>();
        expectClassMetadata<IntegerConverter>();
        expectClassMetadata<LongConverter>();
        expectClassMetadata<DoubleConverter>();
        expectClassMetadata<BooleanConverter>();
        expectClassMetadata<StringFormatter>();
        expectClassMetadata<IntegerFormatter>();
        expectClassMetadata<LongFormatter>();
        expectClassMetadata<DoubleFormatter>();
        expectClassMetadata<BooleanFormatter>();
    }
}
