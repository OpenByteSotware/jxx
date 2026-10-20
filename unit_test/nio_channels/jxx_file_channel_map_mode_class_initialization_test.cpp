#include <gtest/gtest.h>

#include "nio/channels/jxx.nio.channels.ClassInitialization.h"
#include "nio/channels/jxx.nio.channels.FileChannel.h"

namespace
{
    TEST(FileChannelMapModeClassInitialization, RegistersNestedMapModeClass)
    {
        ::jxx::nio::channels::initializeClasses();

        const auto fileChannelType =
            ::jxx::nio::channels::FileChannel::Class();
        ASSERT_NE(fileChannelType, nullptr);

        const auto mapModeType =
            ::jxx::nio::channels::FileChannel::MapMode::Class();
        ASSERT_NE(mapModeType, nullptr);
        ASSERT_NE(mapModeType->getName(), nullptr);
        EXPECT_FALSE(mapModeType->getName()->utf8().empty());
    }
}
