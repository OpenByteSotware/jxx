#include <gtest/gtest.h>

#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.ServiceLoader.h"

TEST(ServiceLoaderParity, LoadCreatesLazyIterable) {
    const auto loader =
        ::jxx::util::ServiceLoader<::jxx::lang::Runnable>::load(
            ::jxx::lang::Runnable::Class());
    ASSERT_NE(nullptr, loader);
    ASSERT_NE(nullptr, loader->iterator());
    EXPECT_NE(std::string::npos, loader->toString()->utf8().find("ServiceLoader"));
}

TEST(ServiceLoaderParity, RejectsNullServiceType) {
    EXPECT_THROW(
        ::jxx::util::ServiceLoader<::jxx::lang::Runnable>::load(nullptr),
        ::jxx::lang::NullPointerException);
}

TEST(ServiceLoaderParity, ReloadInvalidatesExistingIterator) {
    const auto loader =
        ::jxx::util::ServiceLoader<::jxx::lang::Runnable>::load(
            ::jxx::lang::Runnable::Class());
    const auto iteratorValue = loader->iterator();
    loader->reload();
    EXPECT_THROW(
        iteratorValue->hasNext(),
        ::jxx::util::ServiceConfigurationError);
}
