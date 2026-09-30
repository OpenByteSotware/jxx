#include <atomic>
#include <chrono>
#include <thread>
#include <type_traits>
#include <gtest/gtest.h>
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelector.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
namespace {
using namespace std::chrono_literals;
TEST(SelectorHierarchyParity, PublicAndSpiSelectorsAreAbstract){EXPECT_TRUE(std::is_abstract_v<::jxx::nio::channels::Selector>);EXPECT_TRUE(std::is_abstract_v<::jxx::nio::channels::spi::AbstractSelector>);}
TEST(SelectorHierarchyParity, OpenReturnsSpiSelectorFromDefaultProvider){auto provider=::jxx::nio::channels::spi::SelectorProvider::provider();auto selector=::jxx::nio::channels::Selector::open();ASSERT_NE(nullptr,selector);EXPECT_NE(nullptr,::jxx::CAST<::jxx::nio::channels::spi::AbstractSelector>(selector));EXPECT_EQ(provider,selector->provider());selector->close();}
TEST(SelectorHierarchyParity, NativeSelectionAndWakeupRemainOperational){auto selector=::jxx::nio::channels::Selector::open();std::atomic<bool>entered{false};std::atomic<bool>returned{false};std::thread worker([&]{entered=true;EXPECT_EQ(0,selector->select(5000));returned=true;});while(!entered.load())std::this_thread::yield();std::this_thread::sleep_for(50ms);selector->wakeup();worker.join();EXPECT_TRUE(returned.load());selector->close();}
TEST(SelectorHierarchyParity, CloseIsIdempotent){auto selector=::jxx::nio::channels::Selector::open();selector->close();EXPECT_FALSE(selector->isOpen());EXPECT_NO_THROW(selector->close());}
}
