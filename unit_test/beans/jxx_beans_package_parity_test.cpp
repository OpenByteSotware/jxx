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

#include "beans/jxx.beans.ClassInitialization.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.String.h"

namespace {

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
    output->flush();

    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    return ::jxx::CAST<T>(input->readObject());
}

} // namespace

TEST(BeansPackageParity, PackageInitializationRegistersClassMetadata) {
    ::jxx::beans::initialize();

    EXPECT_NE(nullptr, ::jxx::beans::PropertyChangeEvent::Class());
    EXPECT_NE(nullptr, ::jxx::beans::PropertyChangeListener::Class());
    EXPECT_NE(nullptr, ::jxx::beans::PropertyChangeSupport::Class());
}

TEST(BeansPackageParity, SerializationInitializationRegistersSourceSerialVersionUIDs) {
    ::jxx::beans::initialize();

    const auto eventMetadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            ::jxx::beans::PropertyChangeEvent::Class()->getName()->utf8());
    const auto supportMetadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            ::jxx::beans::PropertyChangeSupport::Class()->getName()->utf8());

    EXPECT_EQ(static_cast<::jxx::lang::jlong>(7042693688939648123LL),
        eventMetadata.serialVersionUID);
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(6401253773779951803LL),
        supportMetadata.serialVersionUID);
    EXPECT_TRUE(static_cast<bool>(eventMetadata.deserializationFactory));
    EXPECT_TRUE(static_cast<bool>(supportMetadata.deserializationFactory));
}

TEST(BeansPackageParity, PropertyChangeEventSerializationRoundTripPreservesState) {
    ::jxx::beans::initialize();

    auto event = ::jxx::NEW<::jxx::beans::PropertyChangeEvent>(
        ::jxx::NEW<::jxx::lang::String>("source"),
        ::jxx::NEW<::jxx::lang::String>("property"),
        ::jxx::NEW<::jxx::lang::String>("old"),
        ::jxx::NEW<::jxx::lang::String>("new"));

    auto restored = roundTrip(event);
    ASSERT_NE(nullptr, restored);
    EXPECT_EQ("source",
        ::jxx::CAST<::jxx::lang::String>(restored->getSource())->utf8());
    EXPECT_EQ("property", restored->getPropertyName()->utf8());
    EXPECT_EQ("old",
        ::jxx::CAST<::jxx::lang::String>(restored->getOldValue())->utf8());
    EXPECT_EQ("new",
        ::jxx::CAST<::jxx::lang::String>(restored->getNewValue())->utf8());
}

TEST(BeansPackageParity, PropertyChangeSupportSerializationRoundTripPreservesSource) {
    ::jxx::beans::initialize();

    auto support = ::jxx::NEW<::jxx::beans::PropertyChangeSupport>(
        ::jxx::NEW<::jxx::lang::String>("source"));

    auto restored = roundTrip(support);
    ASSERT_NE(nullptr, restored);

    // A round-trip must leave the restored support object operational.
    restored->firePropertyChange(
        ::jxx::NEW<::jxx::lang::String>("property"),
        ::jxx::NEW<::jxx::lang::String>("old"),
        ::jxx::NEW<::jxx::lang::String>("new"));
    SUCCEED();
}
