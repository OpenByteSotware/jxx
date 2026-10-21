#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/event/jxx.awt.event.ComponentEvent.h"
#include "nio/channels/jxx.nio.channels.Channel.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicInteger.h"

TEST(HierarchicalPackageInitialization, GlobalInitializationReachesNestedPackages)
{
    EXPECT_NO_THROW(::jxx::initialize());
    EXPECT_NE(nullptr, ::jxx::awt::event::ComponentEvent::Class());
    EXPECT_NE(nullptr, ::jxx::nio::channels::Channel::Class());
    EXPECT_NE(nullptr, ::jxx::util::concurrent::atomic::AtomicInteger::Class());
}

TEST(HierarchicalPackageInitialization, GlobalInitializationIsIdempotent)
{
    EXPECT_NO_THROW(::jxx::initialize());
    EXPECT_NO_THROW(::jxx::initialize());
}
