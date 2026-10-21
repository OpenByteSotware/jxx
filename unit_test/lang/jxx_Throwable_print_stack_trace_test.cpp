#include <type_traits>

#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.PrintWriter.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.Throwable.h"

namespace {

using Throwable = ::jxx::lang::Throwable;
using PrintStream = ::jxx::io::PrintStream;
using PrintWriter = ::jxx::io::PrintWriter;

static_assert(std::is_same_v<
    decltype(static_cast<void (Throwable::*)() const>(
        &Throwable::printStackTrace)),
    void (Throwable::*)() const>);
static_assert(std::is_same_v<
    decltype(static_cast<void (Throwable::*)(
        const ::jxx::Ptr<PrintStream>&) const>(
        &Throwable::printStackTrace)),
    void (Throwable::*)(const ::jxx::Ptr<PrintStream>&) const>);
static_assert(std::is_same_v<
    decltype(static_cast<void (Throwable::*)(
        const ::jxx::Ptr<PrintWriter>&) const>(
        &Throwable::printStackTrace)),
    void (Throwable::*)(const ::jxx::Ptr<PrintWriter>&) const>);

TEST(ThrowablePrintStackTraceTest, NoArgumentOverloadUsesSystemErr)
{
    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto stream = ::jxx::NEW<PrintStream>(bytes);
    const auto previous = ::jxx::lang::System::err;
    ::jxx::lang::System::setErr(stream);

    Throwable(::jxx::NEW<::jxx::lang::String>("generated main failure"))
        .printStackTrace();
    stream->flush();

    ::jxx::lang::System::setErr(previous);
    const auto text = bytes->toString();
    ASSERT_NE(nullptr, text);
    EXPECT_NE(std::string::npos, text->utf8().find("generated main failure"));
}

TEST(ThrowablePrintStackTraceTest, PrintStreamAndPrintWriterOverloadsAreSupported)
{
    const auto streamBytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto stream = ::jxx::NEW<PrintStream>(streamBytes);
    Throwable("stream failure").printStackTrace(stream);
    stream->flush();
    EXPECT_NE(std::string::npos,
        streamBytes->toString()->utf8().find("stream failure"));

    const auto writerBytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto writer = ::jxx::NEW<PrintWriter>(writerBytes);
    Throwable("writer failure").printStackTrace(writer);
    writer->flush();
    EXPECT_NE(std::string::npos,
        writerBytes->toString()->utf8().find("writer failure"));
}

TEST(ThrowablePrintStackTraceTest, NullDestinationsThrowNullPointerException)
{
    const Throwable throwable("failure");
    EXPECT_THROW(throwable.printStackTrace(::jxx::Ptr<PrintStream>{}),
        ::jxx::lang::NullPointerException);
    EXPECT_THROW(throwable.printStackTrace(::jxx::Ptr<PrintWriter>{}),
        ::jxx::lang::NullPointerException);
}

} // namespace
