#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "nio/channels/spi/jxx.nio.channels.spi.ClassInitialization.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractInterruptibleChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectableChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectionKey.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelector.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
TEST(JxxNioChannelsSpiClassInitialization, MetadataAndIdempotence)
{
 namespace S=::jxx::nio::channels::spi; namespace C=::jxx::nio::channels;
 ::jxx::initialize();
 EXPECT_NE((::jxx::lang::ClassBase<S::AbstractInterruptibleChannel,::jxx::lang::Object,C::InterruptibleChannel>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<S::AbstractSelectableChannel,C::SelectableChannel>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<S::AbstractSelectionKey,C::SelectionKey>::Class()),nullptr);
 EXPECT_NE((::jxx::lang::ClassBase<S::AbstractSelector,C::Selector>::Class()),nullptr);
 auto before=::jxx::lang::ClassBase<S::SelectorProvider,::jxx::lang::Object>::Class();
 EXPECT_NE(before,nullptr); 
 EXPECT_NO_THROW(S::initializeClasses());
 EXPECT_NO_THROW(S::initializeClasses());
 EXPECT_EQ(before,(::jxx::lang::ClassBase<S::SelectorProvider,::jxx::lang::Object>::Class()));
}
