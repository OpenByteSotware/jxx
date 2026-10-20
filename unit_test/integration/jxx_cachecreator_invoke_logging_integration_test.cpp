#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/jxx.awt.Component.h"
#include "ext/swing/jxx.ext.swing.SwingUtilities.h"
#include "lang/jxx.lang.InvocationTargetException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.RuntimeException.h"
#include "util/logging/jxx.util.logging.Level.h"
#include "util/logging/jxx.util.logging.Logger.h"

namespace {

class ThrowingRunnable final
    : public ::jxx::lang::ClassBase<
          ThrowingRunnable, ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ThrowingRunnable, JxxSuper, ::jxx::lang::Runnable>;

    void run() override
    {
        throw ::jxx::lang::RuntimeException("verification failed");
    }
};

TEST(CacheCreatorSdkIntegration, InvokeFailureCanBeCaughtAndLoggedAtSevere)
{
    ::jxx::initialize();
    auto logger = ::jxx::util::logging::Logger::getLogger(
        ::jxx::awt::Component::Class()->getName());
    ASSERT_NE(logger, nullptr);

    try
    {
        ::jxx::ext::swing::SwingUtilities::invokeAndWait(
            ::jxx::NEW<ThrowingRunnable>());
        FAIL() << "Expected InvocationTargetException";
    }
    catch (const ::jxx::lang::InvocationTargetException& exception)
    {
        ASSERT_NE(exception.getTargetException(), nullptr);
        EXPECT_EQ(exception.getTargetException()->getMessage()->utf8(),
            "verification failed");
        const ::jxx::Ptr<::jxx::lang::String> message = nullptr;
        const ::jxx::Ptr<::jxx::lang::Throwable> thrown =
            ::jxx::CAST<::jxx::lang::Throwable>(
                ::jxx::NEW<::jxx::lang::InvocationTargetException>(exception));
        EXPECT_NO_THROW(logger->log(
            ::jxx::util::logging::Level::SEVERE(),
            message,
            thrown));
    }
}

TEST(CacheCreatorSdkIntegration, InvocationTargetExceptionIsCatchableAsException)
{
    ::jxx::initialize();
    try
    {
        ::jxx::ext::swing::SwingUtilities::invokeAndWait(
            ::jxx::NEW<ThrowingRunnable>());
        FAIL() << "Expected exception";
    }
    catch (const ::jxx::lang::Exception& exception)
    {
        EXPECT_NE(exception.getMessage(), nullptr);
    }
}

} // namespace
