#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Cast.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuilder.h"

TEST(JxxObjectStreamClassDescriptor, ReusesDescriptorAcrossDistinctObjects) {
    auto first = jxx::NEW<jxx::lang::StringBuilder>();
    auto second = jxx::NEW<jxx::lang::StringBuilder>();
    first->append(jxx::NEW<jxx::lang::String>("first"));
    second->append(jxx::NEW<jxx::lang::String>("second"));

    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(first);
    output->writeObject(second);
    output->flush();

    auto input = jxx::NEW<jxx::io::ObjectInputStream>(
        jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restoredFirst = jxx::CAST<jxx::lang::StringBuilder>(input->readObject());
    auto restoredSecond = jxx::CAST<jxx::lang::StringBuilder>(input->readObject());
    EXPECT_EQ("first", restoredFirst->toString()->utf8());
    EXPECT_EQ("second", restoredSecond->toString()->utf8());
}

TEST(JxxObjectStreamClassDescriptor, ResetClearsDescriptorAndObjectTables) {
    auto value = jxx::NEW<jxx::lang::StringBuilder>();
    value->append(jxx::NEW<jxx::lang::String>("value"));
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value);
    output->reset();
    output->writeObject(value);
    output->flush();

    auto input = jxx::NEW<jxx::io::ObjectInputStream>(
        jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto first = jxx::CAST<jxx::lang::StringBuilder>(input->readObject());
    auto second = jxx::CAST<jxx::lang::StringBuilder>(input->readObject());
    EXPECT_EQ("value", first->toString()->utf8());
    EXPECT_EQ("value", second->toString()->utf8());
    EXPECT_NE(first.get(), second.get());
}
