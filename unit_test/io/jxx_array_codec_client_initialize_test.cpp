#include <gtest/gtest.h>
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "jxx.initialize.h"

namespace {
using StringArray = jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>;
using StringMatrix = jxx::lang::JxxArray<jxx::Ptr<jxx::lang::String>, 2U>;
int initializationCount = 0;
void initializeClientTypes() {
    ++initializationCount;
    jxx::io::serialization::registerClassReferenceArrayCodecs<jxx::lang::String, 1U, 2U>();
}
}

TEST(JxxInitialize, RunsEachClientInitializerOnce) {
    jxx::initialize(&initializeClientTypes);
    jxx::initialize(&initializeClientTypes);
    EXPECT_EQ(1, initializationCount);
}

TEST(JxxArrayCodecRegistry, RoundTripsTypedJaggedMatrix) {
    jxx::initialize(&initializeClientTypes);
    auto matrix = jxx::NEW<StringMatrix>(3);
    auto shared = jxx::NEW<StringArray>(2);
    (*shared)[0] = jxx::NEW<jxx::lang::String>("a");
    (*shared)[1] = jxx::NEW<jxx::lang::String>("b");
    (*matrix)[0] = shared; (*matrix)[1] = nullptr; (*matrix)[2] = shared;
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(matrix); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = jxx::CAST<StringMatrix>(input->readObject());
    auto first = static_cast<StringMatrix::RowPointer>((*restored)[0]);
    auto third = static_cast<StringMatrix::RowPointer>((*restored)[2]);
    EXPECT_EQ(nullptr, static_cast<StringMatrix::RowPointer>((*restored)[1]));
    EXPECT_EQ(first.get(), third.get());
    EXPECT_EQ("a", (*first)[0]->utf8());
}
