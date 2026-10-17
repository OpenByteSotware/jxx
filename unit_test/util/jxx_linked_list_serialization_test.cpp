#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.LinkedList.h"

using ErasedLinkedList = ::jxx::util::LinkedList<::jxx::lang::Object>;

TEST(LinkedListSerialization, PreservesOrderAndNulls) {
    ::jxx::initialize();
    auto source = ::jxx::NEW<ErasedLinkedList>();
    source->add(::jxx::NEW<::jxx::lang::String>("first"));
    source->add(nullptr);
    source->add(::jxx::NEW<::jxx::lang::String>("third"));
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(source); output->flush();
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = std::dynamic_pointer_cast<ErasedLinkedList>(input->readObject());
    ASSERT_NE(restored, nullptr); ASSERT_EQ(restored->size(), 3);
    EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(restored->get(0))->utf8(), "first");
    EXPECT_EQ(restored->get(1), nullptr);
    EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(restored->get(2))->utf8(), "third");
}
