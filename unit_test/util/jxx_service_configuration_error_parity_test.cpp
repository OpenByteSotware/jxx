#include <gtest/gtest.h>

#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ServiceConfigurationError.h"

TEST(ServiceConfigurationErrorParity, ExtendsErrorAndPreservesMessage) {
    const auto message = ::jxx::NEW<::jxx::lang::String>("provider failure");
    const auto error =
        ::jxx::NEW<::jxx::util::ServiceConfigurationError>(message);

    const auto asError = ::jxx::CAST<::jxx::lang::Error>(error);
    ASSERT_NE(nullptr, asError);
    ASSERT_NE(nullptr, error->getMessage());
    EXPECT_TRUE(error->getMessage()->equals(message));
}

TEST(ServiceConfigurationErrorParity, PreservesCause) {
    const auto message = ::jxx::NEW<::jxx::lang::String>("provider failure");
    const auto cause = ::jxx::NEW<::jxx::lang::RuntimeException>(
        ::jxx::NEW<::jxx::lang::String>("construction failed"));
    const auto error =
        ::jxx::NEW<::jxx::util::ServiceConfigurationError>(message, cause);

    EXPECT_EQ(cause.get(), error->getCause().get());
    EXPECT_TRUE(error->getMessage()->equals(message));
}
