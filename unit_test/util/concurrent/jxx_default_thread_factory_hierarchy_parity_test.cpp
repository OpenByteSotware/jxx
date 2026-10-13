#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ThreadFactory.h"

namespace {

TEST(DefaultThreadFactoryParity, ConcreteFactoryHasObjectSemantics) {
    using namespace ::jxx::util::concurrent;
    const auto factory = Executors::defaultThreadFactory();
    ASSERT_NE(nullptr, factory);
    EXPECT_NE(nullptr, ::jxx::CAST<::jxx::lang::Object>(factory));
}

TEST(DefaultThreadFactoryParity, ConcreteFactoryMetadataImplementsThreadFactory) {
    using namespace ::jxx::util::concurrent;
    const auto concreteClass =
        executors_detail::DefaultThreadFactory::Class();
    ASSERT_NE(nullptr, concreteClass);
    EXPECT_TRUE(ThreadFactory::Class()->isAssignableFrom(concreteClass));
}

} // namespace
