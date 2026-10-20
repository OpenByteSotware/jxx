#include <gtest/gtest.h>

#include "awt/jxx.awt.ClassInitialization.h"
#include "awt/event/jxx.awt.event.ComponentEvent.h"
#include "awt/event/jxx.awt.event.ContainerEvent.h"
#include "awt/event/jxx.awt.event.FocusEvent.h"
#include "awt/event/jxx.awt.event.HierarchyEvent.h"
#include "awt/event/jxx.awt.event.InputEvent.h"
#include "awt/event/jxx.awt.event.KeyEvent.h"
#include "awt/event/jxx.awt.event.MouseEvent.h"
#include "awt/event/jxx.awt.event.MouseWheelEvent.h"

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

    TEST(AwtEventClassInitialization, RegistersEventClassHierarchy)
    {
        ::jxx::awt::initializeClasses();
        expectClassMetadata<::jxx::awt::event::ComponentEvent>();
        expectClassMetadata<::jxx::awt::event::ContainerEvent>();
        expectClassMetadata<::jxx::awt::event::FocusEvent>();
        expectClassMetadata<::jxx::awt::event::HierarchyEvent>();
        expectClassMetadata<::jxx::awt::event::InputEvent>();
        expectClassMetadata<::jxx::awt::event::KeyEvent>();
        expectClassMetadata<::jxx::awt::event::MouseEvent>();
        expectClassMetadata<::jxx::awt::event::MouseWheelEvent>();
    }
}
