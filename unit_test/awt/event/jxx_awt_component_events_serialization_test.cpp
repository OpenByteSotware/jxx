#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/event/jxx.awt.event.ComponentEvent.h"
#include "awt/event/jxx.awt.event.ContainerEvent.h"
#include "awt/event/jxx.awt.event.FocusEvent.h"
#include "awt/event/jxx.awt.event.HierarchyEvent.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.SerializationClassRegistry.h"

namespace {

template <typename T>
::jxx::Ptr<T> createAndCheckDescriptor(const ::jxx::lang::jlong expectedUid)
{
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            T::Class()->getName()->utf8());
    EXPECT_EQ(expectedUid, metadata.serialVersionUID);
    EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
    return ::jxx::CAST<T>(metadata.deserializationFactory());
}

TEST(JxxAwtComponentEventsSerializationTest, RegistersJava8Descriptors)
{
    ::jxx::initialize();
    EXPECT_NE(nullptr, createAndCheckDescriptor<::jxx::awt::event::ComponentEvent>(
        static_cast<::jxx::lang::jlong>(8101406823902992965LL)));
    EXPECT_NE(nullptr, createAndCheckDescriptor<::jxx::awt::event::ContainerEvent>(
        static_cast<::jxx::lang::jlong>(-4114942250539772041LL)));
    EXPECT_NE(nullptr, createAndCheckDescriptor<::jxx::awt::event::FocusEvent>(
        static_cast<::jxx::lang::jlong>(523753786457416396LL)));
    EXPECT_NE(nullptr, createAndCheckDescriptor<::jxx::awt::event::HierarchyEvent>(
        static_cast<::jxx::lang::jlong>(-5337576970038043990LL)));
}

TEST(JxxAwtComponentEventsSerializationTest, NoDataRestoresSafeDefaults)
{
    ::jxx::initialize();
    const auto containerEvent =
        createAndCheckDescriptor<::jxx::awt::event::ContainerEvent>(
            static_cast<::jxx::lang::jlong>(-4114942250539772041LL));
    containerEvent->readObjectNoData();
    EXPECT_EQ(nullptr, containerEvent->getChild());

    const auto focusEvent = createAndCheckDescriptor<::jxx::awt::event::FocusEvent>(
        static_cast<::jxx::lang::jlong>(523753786457416396LL));
    focusEvent->readObjectNoData();
    EXPECT_FALSE(focusEvent->isTemporary());
    EXPECT_EQ(nullptr, focusEvent->getOppositeComponent());

    const auto hierarchyEvent =
        createAndCheckDescriptor<::jxx::awt::event::HierarchyEvent>(
            static_cast<::jxx::lang::jlong>(-5337576970038043990LL));
    hierarchyEvent->readObjectNoData();
    EXPECT_EQ(nullptr, hierarchyEvent->getChanged());
    EXPECT_EQ(nullptr, hierarchyEvent->getChangedParent());
    EXPECT_EQ(0, hierarchyEvent->getChangeFlags());
}

} // namespace
