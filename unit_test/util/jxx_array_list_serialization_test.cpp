#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"

TEST(ArrayListSerialization, RoundTripsOrderNullsAndElements) {
    using String = ::jxx::lang::String;
    using List = ::jxx::util::ArrayList<::jxx::lang::Object>;

    auto original = ::jxx::NEW<List>();
    original->add(::jxx::NEW<String>("first"));
    original->add(nullptr);
    original->add(::jxx::NEW<String>("third"));

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(original);
    output->close();

    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = ::jxx::CAST<List>(input->readObject());

    ASSERT_NE(nullptr, restored);
    ASSERT_EQ(3, restored->size());
    EXPECT_TRUE(::jxx::CAST<String>(restored->get(0))->equals(
        ::jxx::NEW<String>("first")));
    EXPECT_EQ(nullptr, restored->get(1));
    EXPECT_TRUE(::jxx::CAST<String>(restored->get(2))->equals(
        ::jxx::NEW<String>("third")));
}
