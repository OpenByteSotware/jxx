#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.StringBuilder.h"

TEST(JxxSerializationClassRegistry, UsesRegisteredVersionAndFactory) {
    int factoryCalls = 0;
    jxx::io::serialization::registerSerializableClass<jxx::lang::StringBuilder>(
        7,
        [&factoryCalls] {
            ++factoryCalls;
            return jxx::NEW<jxx::lang::StringBuilder>();
        });
    auto value = jxx::NEW<jxx::lang::StringBuilder>();
    value->append(jxx::NEW<jxx::lang::String>("value"));
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(
        jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = jxx::CAST<jxx::lang::StringBuilder>(input->readObject());
    EXPECT_EQ(1, factoryCalls);
    EXPECT_EQ("value", restored->toString()->utf8());
}
