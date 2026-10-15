#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.InvalidClassException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuilder.h"

namespace {
jxx::lang::ByteArray serializeBuilder() {
    auto value = jxx::NEW<jxx::lang::StringBuilder>();
    value->append(jxx::NEW<jxx::lang::String>("value"));
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value); output->flush(); return bytes->toByteArray();
}
}

TEST(JxxObjectInputClassFilter, RejectsDisallowedObjectBeforeConstruction) {
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(
        jxx::NEW<jxx::io::ByteArrayInputStream>(serializeBuilder()));
    input->setClassFilter([](const std::string& binaryName) {
        return binaryName != "jxx.lang.StringBuilder";
    });
    EXPECT_TRUE(input->hasClassFilter());
    EXPECT_THROW(input->readObject(), jxx::io::InvalidClassException);
}

TEST(JxxObjectInputClassFilter, AllowsApprovedObject) {
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(
        jxx::NEW<jxx::io::ByteArrayInputStream>(serializeBuilder()));
    input->setClassFilter([](const std::string& binaryName) {
        return binaryName == "jxx.lang.StringBuilder";
    });
    auto restored = jxx::CAST<jxx::lang::StringBuilder>(input->readObject());
    EXPECT_EQ("value", restored->toString()->utf8());
    input->clearClassFilter();
    EXPECT_FALSE(input->hasClassFilter());
}
