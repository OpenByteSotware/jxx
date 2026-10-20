#include <gtest/gtest.h>

#include "ext/imageio/jxx.ext.imageio.ClassInitialization.h"
#include "ext/imageio/jxx.ext.imageio.IIOImage.h"
#include "ext/imageio/jxx.ext.imageio.ImageWriteParam.h"
#include "ext/imageio/jxx.ext.imageio.ImageWriter.h"
#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegImageWriter.h"
#include "lang/jxx.lang.String.h"

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

    TEST(ImageIoClassInitialization, RegistersWriterImplementation)
    {
        ::jxx::ext::imageio::initializeClasses();
        expectClassMetadata<::jxx::ext::imageio::IIOImage>();
        expectClassMetadata<::jxx::ext::imageio::ImageWriteParam>();
        expectClassMetadata<::jxx::ext::imageio::ImageWriter>();
        expectClassMetadata<::jxx::ext::imageio::internal::JpegImageWriter>();
    }
}
