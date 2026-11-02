#include <gtest/gtest.h>
#include <type_traits>
#include "beans/jxx.beans.PropertyChangeEvent.h"
#include "beans/jxx.beans.PropertyChangeListener.h"
#include "util/jxx.util.EventListener.h"
#include "util/jxx.util.EventObject.h"

TEST(BeansInheritanceParity, PropertyChangeEventExtendsEventObject) {
    static_assert(std::is_base_of_v<::jxx::util::EventObject, ::jxx::beans::PropertyChangeEvent>);
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::EventObject, ::jxx::beans::PropertyChangeEvent>));
}

TEST(BeansInheritanceParity, PropertyChangeListenerExtendsEventListener) {
    static_assert(std::is_base_of_v<::jxx::util::EventListener, ::jxx::beans::PropertyChangeListener>);
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::EventListener, ::jxx::beans::PropertyChangeListener>));
}
