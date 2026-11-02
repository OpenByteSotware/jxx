#include <gtest/gtest.h>
#include <type_traits>

#include "beans/jxx.beans.PropertyChangeEvent.h"
#include "beans/jxx.beans.PropertyChangeListener.h"
#include "beans/jxx.beans.PropertyChangeSupport.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.EventListener.h"
#include "util/jxx.util.EventObject.h"

TEST(BeansPackageParity, PublicHierarchyMatchesSourceTypes) {
    static_assert(std::is_base_of_v<::jxx::util::EventObject, ::jxx::beans::PropertyChangeEvent>);
    static_assert(std::is_base_of_v<::jxx::util::EventListener, ::jxx::beans::PropertyChangeListener>);
    static_assert(std::is_base_of_v<::jxx::lang::Object, ::jxx::beans::PropertyChangeSupport>);
    SUCCEED();
}

TEST(BeansPackageParity, SerializableTypesExposeSerializationContract) {
    static_assert(std::is_base_of_v<::jxx::io::SerializableI, ::jxx::beans::PropertyChangeEvent>);
    static_assert(std::is_base_of_v<::jxx::io::SerializableI, ::jxx::beans::PropertyChangeSupport>);
    SUCCEED();
}

TEST(BeansPackageParity, PublicClassesRemainExtensible) {
    static_assert(!std::is_final_v<::jxx::beans::PropertyChangeEvent>);
    static_assert(!std::is_final_v<::jxx::beans::PropertyChangeSupport>);
    SUCCEED();
}

TEST(BeansPackageParity, PublicTypesExposeClassMetadata) {
    EXPECT_NE(nullptr, ::jxx::beans::PropertyChangeEvent::Class());
    EXPECT_NE(nullptr, ::jxx::beans::PropertyChangeListener::Class());
    EXPECT_NE(nullptr, ::jxx::beans::PropertyChangeSupport::Class());
}
