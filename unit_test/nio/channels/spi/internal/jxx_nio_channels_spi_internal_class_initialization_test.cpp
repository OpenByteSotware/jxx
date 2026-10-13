#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.ClassInitialization.h"
#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelectionKey.h"
#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelectorProvider.h"
TEST(JxxNioChannelsSpiInternalClassInitialization, MetadataAndIdempotence)
{
 namespace I=::jxx::nio::channels::spi::internal; namespace S=::jxx::nio::channels::spi;
 ::jxx::initialize();
 auto key=::jxx::lang::ClassBase<I::NativeSelectionKey,S::AbstractSelectionKey>::Class();
 auto provider=::jxx::lang::ClassBase<I::NativeSelectorProvider,S::SelectorProvider>::Class();
 EXPECT_NE(key,nullptr); EXPECT_NE(provider,nullptr); EXPECT_NE(key,provider);
 EXPECT_NO_THROW(I::initializeClasses()); 
 EXPECT_NO_THROW(I::initializeClasses());
 EXPECT_EQ(key,(::jxx::lang::ClassBase<I::NativeSelectionKey,S::AbstractSelectionKey>::Class()));
}
