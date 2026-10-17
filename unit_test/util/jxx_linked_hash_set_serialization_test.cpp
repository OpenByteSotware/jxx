#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.LinkedHashSet.h"

using ErasedLinkedHashSet = ::jxx::util::LinkedHashSet<::jxx::lang::Object>;

TEST(LinkedHashSetSerialization, PreservesInsertionOrder) {
    ::jxx::initialize();
    auto source = ::jxx::NEW<ErasedLinkedHashSet>();
    source->add(::jxx::NEW<::jxx::lang::String>("first"));
    source->add(::jxx::NEW<::jxx::lang::String>("second"));
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(source); output->flush();
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = std::dynamic_pointer_cast<ErasedLinkedHashSet>(input->readObject());
    ASSERT_NE(restored, nullptr);
    auto iterator = restored->iterator();
    EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(iterator->next())->utf8(), "first");
    EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(iterator->next())->utf8(), "second");
}
