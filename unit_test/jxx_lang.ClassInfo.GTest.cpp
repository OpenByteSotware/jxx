#include <type_traits>

#include <gtest/gtest.h>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.InstantiationException.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::test::classinfo
{

    // -----------------------------------------------------------------------------
    // Interfaces
    // -----------------------------------------------------------------------------

    class Startable
        : public jxx::lang::ClassInfo<Startable>
    {
    private:
        using Metadata =
            jxx::lang::ClassInfo<Startable>;
    public:
        using JxxClassInfoMarker =
            Metadata;

        using Metadata::Class;

        virtual ~Startable() = default;

        virtual void start() = 0;
    };

    class Stoppable
        : public jxx::lang::ClassInfo<Stoppable>
    {
    private:
        using Metadata =
            jxx::lang::ClassInfo<Stoppable>;
    public:
        using JxxClassInfoMarker = Metadata;

        using Metadata::Class;

        virtual ~Stoppable() = default;

        virtual void stop() = 0;
    };

    /** Java-style marker interface with no methods. */
    class Resettable
        : public jxx::lang::ClassInfo<Resettable>
    {
    private:
        using Metadata =
            jxx::lang::ClassInfo<Resettable>;
    public:
        using JxxClassInfoMarker = Metadata;

        using Metadata::Class;

        virtual ~Resettable() = default;
    };

    /** Interface extending multiple interfaces. */
    class ManagedDevice
        : public jxx::lang::ClassInfo<
        ManagedDevice, jxx::lang::Object,
        Startable,
        Stoppable>
        , public virtual Startable
        , public virtual Stoppable
    {
    private:
        using Metadata =
            jxx::lang::ClassInfo<
            ManagedDevice, jxx::lang::Object,
            Startable,
            Stoppable>;

    public:
        using JxxClassInfoMarker = Metadata;

        using Metadata::Class;

        virtual ~ManagedDevice() = default;

        virtual void synchronize() = 0;
    };

    // -----------------------------------------------------------------------------
    // Object-derived classes
    // -----------------------------------------------------------------------------

    class PlainDevice final
        : public jxx::lang::Object
        , public jxx::lang::ClassInfo<PlainDevice>
    {
    private:
        using Metadata =
            jxx::lang::ClassInfo<PlainDevice>;
    public:
        using JxxClassInfoMarker = Metadata;

        using Metadata::Class;

        PlainDevice() = default;

    protected:
        JXX_OBJECT_CLONE(PlainDevice)
    };

    class AbstractDevice
        : public jxx::lang::Object
        , public jxx::lang::ClassInfo<
        AbstractDevice,
        jxx::lang::Object,
        Startable>
        , public virtual Startable
    {
    private:
        using Metadata =
            jxx::lang::ClassInfo<
            AbstractDevice,
            jxx::lang::Object,
            Startable>;

    public:
        using JxxClassInfoMarker = Metadata;

        using Metadata::Class;

        virtual ~AbstractDevice() = default;

        void start() override = 0;
    };

    class ConcreteDevice final
        : public AbstractDevice
        , public jxx::lang::ClassInfo<
        ConcreteDevice,
        AbstractDevice,
        Stoppable,
        Resettable>
        , public virtual Stoppable
        , public virtual Resettable
    {
    private:
        using Metadata = jxx::lang::ClassInfo<
            ConcreteDevice,
            AbstractDevice,
            Stoppable,
            Resettable>;
    
    public:
        using JxxClassInfoMarker = Metadata;
        using Metadata::Class;
        ConcreteDevice() = default;
        ~ConcreteDevice() override = default;        
    protected:
        JXX_OBJECT_CLONE(ConcreteDevice)
    private:
        bool started_ = false;    
    public:              

        void start() override
        {
            started_ = true;
        }

        void stop() override
        {
            started_ = false;
        }

        bool isStarted() const
        {
            return started_;
        }   
    };

    /** Concrete class directly implementing a multiply-derived interface. */
    class ManagedNetworkDevice final
        : public jxx::lang::Object
        , public jxx::lang::ClassInfo<
        ManagedNetworkDevice,
        jxx::lang::Object,
        ManagedDevice,
        Resettable>
        , public virtual ManagedDevice
        , public virtual Resettable
    {
    private:
        using Metadata =
            jxx::lang::ClassInfo<
            ManagedNetworkDevice,
            jxx::lang::Object,
            ManagedDevice,
            Resettable>;

    public:
        using JxxClassInfoMarker = Metadata;
        using Metadata::Class;

        ManagedNetworkDevice() = default;

        void start() override
        {
            running_ = true;
        }

        void stop() override
        {
            running_ = false;
        }

        void synchronize() override
        {
            synchronized_ = true;
        }

        bool isRunning() const
        {
            return running_;
        }

        bool isSynchronized() const
        {
            return synchronized_;
        }

    protected:
        JXX_OBJECT_CLONE(ManagedNetworkDevice)

    private:
        bool running_ = false;
        bool synchronized_ = false;
    };

    /** No public/default constructor, so ClassAny has no construction factory. */
    class NoDefaultConstructor final
        : public jxx::lang::Object
        , public jxx::lang::ClassInfo<NoDefaultConstructor, jxx::lang::Object>
    {
    private:
        using Metadata =
            jxx::lang::ClassInfo<NoDefaultConstructor, jxx::lang::Object>;
    public:
        using JxxClassInfoMarker = Metadata;

        using Metadata::Class;

        explicit NoDefaultConstructor(
            jxx::lang::jint value)
            : value_(value)
        {
        }

        jxx::lang::jint value() const
        {
            return value_;
        }

    protected:
        JXX_OBJECT_CLONE(NoDefaultConstructor)

    private:
        jxx::lang::jint value_;
    };

} // namespace jxx::test::classinfo

namespace
{

    using jxx::test::classinfo::AbstractDevice;
    using jxx::test::classinfo::ConcreteDevice;
    using jxx::test::classinfo::ManagedDevice;
    using jxx::test::classinfo::ManagedNetworkDevice;
    using jxx::test::classinfo::NoDefaultConstructor;
    using jxx::test::classinfo::PlainDevice;
    using jxx::test::classinfo::Resettable;
    using jxx::test::classinfo::Startable;
    using jxx::test::classinfo::Stoppable;

    constexpr jxx::lang::jint PUBLIC_MODIFIER = 0x0001;
    constexpr jxx::lang::jint INTERFACE_MODIFIER = 0x0200;
    constexpr jxx::lang::jint ABSTRACT_MODIFIER = 0x0400;

    TEST(ClassInfoVariadicTest, MetadataMixinDoesNotMakeInterfaceAnObject)
    {
        static_assert(!std::is_base_of_v<jxx::lang::Object, Startable>);
        static_assert(!std::is_base_of_v<jxx::lang::Object, Stoppable>);
        static_assert(!std::is_base_of_v<jxx::lang::Object, Resettable>);

        SUCCEED();
    }

    TEST(ClassInfoVariadicTest, MetadataMixinDoesNotAddPolymorphismRequirementToMarkerInterface)
    {
        static_assert(std::is_polymorphic_v<Resettable>);
        static_assert(!std::is_abstract_v<Resettable>);

        auto descriptor = Resettable::Class();

        ASSERT_NE(descriptor, nullptr);
        EXPECT_TRUE(descriptor->isInterface());
    }

    TEST(ClassInfoVariadicTest, PlainObjectSubclassRegistersAsClass)
    {
        auto descriptor = PlainDevice::Class();

        ASSERT_NE(descriptor, nullptr);
        EXPECT_FALSE(descriptor->isInterface());
        EXPECT_FALSE(descriptor->isPrimitive());
        EXPECT_FALSE(descriptor->isArray());
        EXPECT_EQ(descriptor->getSuperclass(), jxx::lang::class_info_detail::ensureObjectRegistered());
        EXPECT_NE(descriptor->getModifiers() & PUBLIC_MODIFIER, 0);
    }

    TEST(ClassInfoVariadicTest, GeneratedClassNamesContainNamespaceAndSimpleName)
    {
        auto descriptor = PlainDevice::Class();

        ASSERT_NE(descriptor, nullptr);
        ASSERT_NE(descriptor->getName(), nullptr);
        ASSERT_NE(descriptor->getSimpleName(), nullptr);

        EXPECT_NE(
            descriptor->getName()->utf8().find("jxx.test.classinfo.PlainDevice"),
            std::string::npos);

        EXPECT_EQ(
            descriptor->getSimpleName()->utf8(),
            "PlainDevice");
    }

    TEST(ClassInfoVariadicTest, ClassDescriptorIsCanonical)
    {
        EXPECT_EQ(PlainDevice::Class(), PlainDevice::Class());
        EXPECT_EQ(Startable::Class(), Startable::Class());
    }

    TEST(ClassInfoVariadicTest, InterfaceHasJavaInterfaceMetadata)
    {
        auto descriptor = Startable::Class();

        ASSERT_NE(descriptor, nullptr);
        EXPECT_TRUE(descriptor->isInterface());
        EXPECT_EQ(descriptor->getSuperclass(), nullptr);
        EXPECT_NE(descriptor->getModifiers() & PUBLIC_MODIFIER, 0);
        EXPECT_NE(descriptor->getModifiers() & INTERFACE_MODIFIER, 0);
        EXPECT_NE(descriptor->getModifiers() & ABSTRACT_MODIFIER, 0);
    }

    TEST(ClassInfoVariadicTest, MarkerInterfaceCannotBeInstantiated)
    {
        EXPECT_THROW(
            Resettable::Class()->newInstance(),
            jxx::lang::InstantiationException);
    }

    TEST(ClassInfoVariadicTest, InterfaceExtendsMultipleParentInterfaces)
    {
        auto descriptor = ManagedDevice::Class();
        auto interfaces = descriptor->getInterfaces();

        ASSERT_NE(interfaces, nullptr);
        ASSERT_EQ(interfaces->size(), 2U);

        EXPECT_EQ((*interfaces)[0], Startable::Class());
        EXPECT_EQ((*interfaces)[1], Stoppable::Class());
    }

    TEST(ClassInfoVariadicTest, ConcreteClassRegistersMultipleDirectInterfaces)
    {
        auto descriptor = ConcreteDevice::Class();
        auto interfaces = descriptor->getInterfaces();

        ASSERT_NE(interfaces, nullptr);
        ASSERT_EQ(interfaces->size(), 2U);

        EXPECT_EQ((*interfaces)[0], Stoppable::Class());
        EXPECT_EQ((*interfaces)[1], Resettable::Class());
    }

    TEST(ClassInfoVariadicTest, ConcreteClassRegistersDeclaredSuperclass)
    {
        EXPECT_EQ(
            ConcreteDevice::Class()->getSuperclass(),
            AbstractDevice::Class());
    }

    TEST(ClassInfoVariadicTest, AbstractClassIsRegisteredWithoutFactory)
    {
        auto descriptor = AbstractDevice::Class();

        ASSERT_NE(descriptor, nullptr);
        EXPECT_FALSE(descriptor->isInterface());
        EXPECT_NE(descriptor->getModifiers() & ABSTRACT_MODIFIER, 0);

        EXPECT_THROW(
            descriptor->newInstance(),
            jxx::lang::InstantiationException);
    }

    TEST(ClassInfoVariadicTest, ConcreteClassCanBeCreatedByClassDescriptor)
    {
        auto object = ConcreteDevice::Class()->newInstance();
        auto device = jxx::CAST<ConcreteDevice>(object);

        ASSERT_NE(object, nullptr);
        ASSERT_NE(device, nullptr);
        EXPECT_EQ(object->getClass(), ConcreteDevice::Class());
    }

    TEST(ClassInfoVariadicTest, ClassWithoutDefaultConstructorHasNoFactory)
    {
        auto descriptor = NoDefaultConstructor::Class();

        ASSERT_NE(descriptor, nullptr);

        EXPECT_THROW(
            descriptor->newInstance(),
            jxx::lang::InstantiationException);

        auto value = jxx::NEW<NoDefaultConstructor>(42);
        ASSERT_NE(value, nullptr);
        EXPECT_EQ(value->value(), 42);
    }

    TEST(ClassInfoVariadicTest, ObjectGetClassUsesRuntimeTypeRegistration)
    {
        auto device = jxx::NEW<ManagedNetworkDevice>();
        auto object = jxx::CAST<jxx::lang::Object>(device);

        ASSERT_NE(object, nullptr);
        EXPECT_EQ(object->getClass(), ManagedNetworkDevice::Class());
    }

    TEST(ClassInfoVariadicTest, DirectInterfaceIsInstanceOfConcreteObject)
    {
        auto value = jxx::NEW<ManagedNetworkDevice>();
        auto object = jxx::CAST<jxx::lang::Object>(value);

        EXPECT_TRUE(ManagedDevice::Class()->isInstance(object));
        EXPECT_TRUE(Resettable::Class()->isInstance(object));
    }

    TEST(ClassInfoVariadicTest, ParentInterfacesAreInstanceOfConcreteObject)
    {
        auto value = jxx::NEW<ManagedNetworkDevice>();
        auto object = jxx::CAST<jxx::lang::Object>(value);

        EXPECT_TRUE(Startable::Class()->isInstance(object));
        EXPECT_TRUE(Stoppable::Class()->isInstance(object));
    }

    TEST(ClassInfoVariadicTest, InterfaceCrossCastsWorkAtRuntime)
    {
        auto value = jxx::NEW<ManagedNetworkDevice>();
        auto object = jxx::CAST<jxx::lang::Object>(value);

        auto startable = std::dynamic_pointer_cast<Startable>(object);
        auto stoppable = std::dynamic_pointer_cast<Stoppable>(object);
        auto managed = std::dynamic_pointer_cast<ManagedDevice>(object);
        auto resettable = std::dynamic_pointer_cast<Resettable>(object);

        EXPECT_NE(startable, nullptr);
        EXPECT_NE(stoppable, nullptr);
        EXPECT_NE(managed, nullptr);
        EXPECT_NE(resettable, nullptr);
    }

    TEST(ClassInfoVariadicTest, InterfaceAssignabilityFollowsParentInterfaces)
    {
        EXPECT_TRUE(
            Startable::Class()->isAssignableFrom(
                ManagedDevice::Class()));

        EXPECT_TRUE(
            Stoppable::Class()->isAssignableFrom(
                ManagedDevice::Class()));

        EXPECT_FALSE(
            ManagedDevice::Class()->isAssignableFrom(
                Startable::Class()));
    }

    TEST(ClassInfoVariadicTest, InterfaceAssignabilityFollowsConcreteClassMetadata)
    {
        EXPECT_TRUE(
            ManagedDevice::Class()->isAssignableFrom(
                ManagedNetworkDevice::Class()));

        EXPECT_TRUE(
            Startable::Class()->isAssignableFrom(
                ManagedNetworkDevice::Class()));

        EXPECT_TRUE(
            Stoppable::Class()->isAssignableFrom(
                ManagedNetworkDevice::Class()));

        EXPECT_TRUE(
            Resettable::Class()->isAssignableFrom(
                ManagedNetworkDevice::Class()));
    }

    TEST(ClassInfoVariadicTest, SuperclassInterfacesAreInheritedBySubclass)
    {
        EXPECT_TRUE(
            Startable::Class()->isAssignableFrom(
                ConcreteDevice::Class()));

        auto value = jxx::NEW<ConcreteDevice>();
        auto object = jxx::CAST<jxx::lang::Object>(value);

        EXPECT_TRUE(Startable::Class()->isInstance(object));
        EXPECT_TRUE(Stoppable::Class()->isInstance(object));
        EXPECT_TRUE(Resettable::Class()->isInstance(object));
    }

    TEST(ClassInfoVariadicTest, UnrelatedInterfaceDoesNotMatch)
    {
        auto value = jxx::NEW<PlainDevice>();
        auto object = jxx::CAST<jxx::lang::Object>(value);

        EXPECT_FALSE(Startable::Class()->isInstance(object));
        EXPECT_FALSE(ManagedDevice::Class()->isInstance(object));
    }

    TEST(ClassInfoVariadicTest, InterfaceMethodDispatchWorksAfterCrossCast)
    {
        auto value = jxx::NEW<ManagedNetworkDevice>();
        auto object = jxx::CAST<jxx::lang::Object>(value);

        auto startable = std::dynamic_pointer_cast<Startable>(object);
        auto stoppable = std::dynamic_pointer_cast<Stoppable>(object);
        auto managed = std::dynamic_pointer_cast<ManagedDevice>(object);

        ASSERT_NE(startable, nullptr);
        ASSERT_NE(stoppable, nullptr);
        ASSERT_NE(managed, nullptr);

        startable->start();
        EXPECT_TRUE(value->isRunning());

        managed->synchronize();
        EXPECT_TRUE(value->isSynchronized());

        stoppable->stop();
        EXPECT_FALSE(value->isRunning());
    }

} // namespace
