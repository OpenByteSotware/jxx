#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Cast.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

TEST(JxxObjectStreamStage6, RoundTripsEnumAsCanonicalSingleton) {
    auto value = jxx::util::concurrent::TimeUnit::SECONDS();
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value); output->writeObject(value); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto first = jxx::CAST<jxx::util::concurrent::TimeUnit>(input->readObject());
    auto second = jxx::CAST<jxx::util::concurrent::TimeUnit>(input->readObject());
    EXPECT_EQ(value.get(), first.get());
    EXPECT_EQ(first.get(), second.get());
}

TEST(JxxObjectStreamStage6, RoundTripsCanonicalClassAny) {
    auto value = jxx::util::concurrent::TimeUnit::Class();
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value); output->writeObject(value); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto first = jxx::CAST<jxx::lang::ClassAny>(input->readObject());
    auto second = jxx::CAST<jxx::lang::ClassAny>(input->readObject());
    EXPECT_EQ(value.get(), first.get());
    EXPECT_EQ(first.get(), second.get());
    EXPECT_EQ(value->getName()->utf8(), first->getName()->utf8());
}
