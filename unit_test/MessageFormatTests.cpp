#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.MessageFormat.h"
#include "tests/TestHelpers.h"

namespace {
using namespace jxx::tests;
using jxx::lang::Object;
using jxx::lang::String;
using jxx::util::MessageFormat;

TEST(MessageFormatTests, ReplacesIndexedPlaceholders) {
    auto fmt = MessageFormat::of(String::valueOf("Hello {0}"));
    jxx::Ptr<Object> args[1] = { std::static_pointer_cast<Object>(String::valueOf("James")) };
    auto out = fmt->format(args, 1);
    ExpectStringEq(out, "Hello James");
}

TEST(MessageFormatTests, SupportsMultipleArguments) {
    auto fmt = MessageFormat::of(String::valueOf("{0} has {1} items"));
    jxx::Ptr<Object> args[2] = {
        std::static_pointer_cast<Object>(String::valueOf("James")),
        std::static_pointer_cast<Object>(String::valueOf("3"))
    };
    auto out = fmt->format(args, 2);
    ExpectStringEq(out, "James has 3 items");
}

TEST(MessageFormatTests, LeavesMissingIndexesUntouched) {
    auto fmt = MessageFormat::of(String::valueOf("Value={1}"));
    jxx::Ptr<Object> args[1] = { std::static_pointer_cast<Object>(String::valueOf("x")) };
    auto out = fmt->format(args, 1);
    ExpectStringEq(out, "Value={1}");
}

} // namespace
