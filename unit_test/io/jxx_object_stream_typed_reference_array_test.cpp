#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Cast.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuilder.h"
#include "lang/jxx.lang.JxxArray.h"

namespace {
using StringArray = jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>;
}

TEST(JxxObjectStreamStage7, RoundTripsTypedStringArray) {
    auto value = jxx::NEW<StringArray>(4);
    auto shared = jxx::NEW<jxx::lang::String>("shared");
    (*value)[0] = shared; (*value)[1] = nullptr; (*value)[2] = shared;
    (*value)[3] = jxx::NEW<jxx::lang::String>("last");
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = jxx::CAST<StringArray>(input->readObject());
    ASSERT_EQ(4, restored->length);
    EXPECT_EQ(nullptr, (*restored)[1]);
    EXPECT_EQ((*restored)[0].get(), (*restored)[2].get());
    EXPECT_EQ("last", (*restored)[3]->utf8());
}

TEST(JxxObjectStreamStage7, PreservesConcreteArrayType) {
    auto value = jxx::NEW<StringArray>(1);
    (*value)[0] = jxx::NEW<jxx::lang::String>("typed");
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto object = input->readObject();
    EXPECT_NE(nullptr, std::dynamic_pointer_cast<StringArray>(object));
    EXPECT_EQ(nullptr, std::dynamic_pointer_cast<jxx::lang::ObjectArrayType>(object));
}
