#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.String.h"
#include "util/logging/jxx.util.logging.ClassInitialization.h"
#include "util/logging/jxx.util.logging.Filter.h"
#include "util/logging/jxx.util.logging.Formatter.h"
#include "util/logging/jxx.util.logging.Handler.h"
#include "util/logging/jxx.util.logging.Level.h"
#include "util/logging/jxx.util.logging.LogRecord.h"
#include "util/logging/jxx.util.logging.Logger.h"

namespace logging = ::jxx::util::logging;

TEST(LoggingPackageParity, PublicTypeKindsAndInheritanceRemainCorrect) {
    static_assert(std::is_abstract_v<logging::Filter>);
    static_assert(std::is_abstract_v<logging::Formatter>);
    static_assert(std::is_abstract_v<logging::Handler>);
    static_assert(std::is_base_of_v<::jxx::io::SerializableI, logging::Level>);
    SUCCEED();
}

TEST(LoggingPackageParity, PackageInitializationRegistersImplementedTypes) {
    logging::initialize();
    EXPECT_NE(nullptr, logging::Filter::Class());
    EXPECT_NE(nullptr, logging::Formatter::Class());
    EXPECT_NE(nullptr, logging::Handler::Class());
    EXPECT_NE(nullptr, logging::Level::Class());
    EXPECT_NE(nullptr, logging::LogRecord::Class());
    EXPECT_NE(nullptr, logging::Logger::Class());
}

TEST(LoggingPackageParity, LevelSerializationRegistrationUsesSourceUID) {
    logging::initialize();
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            logging::Level::Class()->getName()->utf8());
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(-8176160795706313070LL),
        metadata.serialVersionUID);
    EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
}

TEST(LoggingPackageParity, LevelSerializationRoundTripPreservesState) {
    logging::initialize();
    auto original = ::jxx::NEW<logging::Level>(
        ::jxx::NEW<::jxx::lang::String>("CUSTOM"), 1234,
        ::jxx::NEW<::jxx::lang::String>("bundle.name"));
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(original));
    output->flush();
    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    auto restored = ::jxx::CAST<logging::Level>(input->readObject());
    ASSERT_NE(nullptr, restored);
    EXPECT_EQ("CUSTOM", restored->getName()->utf8());
    EXPECT_EQ(1234, restored->intValue());
    ASSERT_NE(nullptr, restored->getResourceBundleName());
    EXPECT_EQ("bundle.name", restored->getResourceBundleName()->utf8());
}
