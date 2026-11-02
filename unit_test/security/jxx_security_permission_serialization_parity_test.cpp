#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.String.h"
#include "security/jxx.security.BasicPermission.h"
#include "security/jxx.security.ClassInitialization.h"
#include "security/jxx.security.Guard.h"
#include "security/jxx.security.Permission.h"

namespace security = ::jxx::security;

TEST(SecurityPermissionInheritanceParity, PermissionImplementsGuardAndSerializable) {
    static_assert(std::is_base_of_v<security::Guard, security::Permission>);
    static_assert(std::is_base_of_v<::jxx::io::SerializableI, security::Permission>);
    static_assert(std::is_base_of_v<security::Permission, security::BasicPermission>);
    SUCCEED();
}

TEST(SecurityPermissionInitializationParity, RegistersPermissionHierarchy) {
    security::initializeClasses();
    EXPECT_NE(nullptr, security::Guard::Class());
    EXPECT_NE(nullptr, security::Permission::Class());
    EXPECT_NE(nullptr, security::BasicPermission::Class());
}

TEST(SecurityPermissionSerializationParity, BasicPermissionUsesSourceSerialVersionUID) {
    security::initializeClasses();
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            security::BasicPermission::Class()->getName()->utf8());
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(6279438298436773498LL),
        metadata.serialVersionUID);
    EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
}

TEST(SecurityPermissionSerializationParity, BasicPermissionRoundTripPreservesNameAndWildcard) {
    security::initializeClasses();
    auto original = ::jxx::NEW<security::BasicPermission>(
        ::jxx::NEW<::jxx::lang::String>("service.*"));

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(original));
    output->flush();

    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    auto restored = ::jxx::CAST<security::BasicPermission>(input->readObject());

    ASSERT_NE(nullptr, restored);
    EXPECT_EQ("service.*", restored->getName()->utf8());
    EXPECT_TRUE(restored->implies(::jxx::NEW<security::BasicPermission>(
        ::jxx::NEW<::jxx::lang::String>("service.read"))));
    EXPECT_FALSE(restored->implies(::jxx::NEW<security::BasicPermission>(
        ::jxx::NEW<::jxx::lang::String>("other.read"))));
}
