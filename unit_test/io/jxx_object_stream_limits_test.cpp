#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.StreamCorruptedException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.JxxArray.h"

namespace {
jxx::lang::ByteArray serialize(const jxx::Ptr<jxx::lang::Object>& value) {
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value); output->flush(); return bytes->toByteArray();
}
}

TEST(JxxObjectStreamLimits, RejectsStringAboveConfiguredLimit) {
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(serialize(jxx::NEW<jxx::lang::String>("abcd"))));
    auto limits = input->getLimits(); limits.maxStringCodeUnits = 3; input->setLimits(limits);
    EXPECT_THROW(input->readObject(), jxx::io::StreamCorruptedException);
}

TEST(JxxObjectStreamLimits, RejectsArrayAboveConfiguredLimit) {
    auto value = jxx::NEW<jxx::lang::IntArrayType>(4);
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(serialize(value)));
    auto limits = input->getLimits(); limits.maxArrayElements = 3; input->setLimits(limits);
    EXPECT_THROW(input->readObject(), jxx::io::StreamCorruptedException);
}

TEST(JxxObjectStreamLimits, RejectsNestedGraphAboveDepthLimit) {
    auto outer = jxx::NEW<jxx::lang::ObjectArrayType>(1);
    auto inner = jxx::NEW<jxx::lang::ObjectArrayType>(1);
    (*outer)[0] = inner; (*inner)[0] = jxx::NEW<jxx::lang::String>("leaf");
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(jxx::NEW<jxx::io::ByteArrayInputStream>(serialize(outer)));
    auto limits = input->getLimits(); limits.maxGraphDepth = 2; input->setLimits(limits);
    EXPECT_THROW(input->readObject(), jxx::io::StreamCorruptedException);
}
