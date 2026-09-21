#include <gtest/gtest.h>

#include <thread>
#include <vector>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentHashMap.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentMap.h"
#include "util/function/jxx.util.function.Function.h"

namespace {
using S = ::jxx::lang::String;
using MapType = ::jxx::util::concurrent::ConcurrentHashMap<S, S>;

class IdentityValue final : public ::jxx::util::function::Function<S, S> {
public:
    ::jxx::Ptr<S> apply(const ::jxx::Ptr<S> key) override {
        return ::jxx::NEW<S>(key->utf8());
    }
};

TEST(ConcurrentHashMapParityTest, ConstructorsAndConcurrentPutIfAbsentAreAtomic) {
    const auto map = ::jxx::NEW<MapType>(32, 0.75F, 4);
    const auto key = ::jxx::NEW<S>("shared");
    std::vector<std::thread> threads;
    for (int index = 0; index < 8; ++index) {
        threads.emplace_back([map, key, index] {
            map->putIfAbsent(key, ::jxx::NEW<S>(std::to_string(index)));
        });
    }
    for (auto& thread : threads) thread.join();
    EXPECT_EQ(1, map->size());
    ASSERT_NE(nullptr, map->get(::jxx::CAST<::jxx::lang::Object>(key)));
}

TEST(ConcurrentHashMapParityTest, ComputeIfAbsentExecutesUnderAtomicUpdate) {
    const auto map = ::jxx::NEW<MapType>();
    const auto key = ::jxx::NEW<S>("computed");
    const auto function = ::jxx::NEW<IdentityValue>();
    std::vector<std::thread> threads;
    for (int index = 0; index < 8; ++index) {
        threads.emplace_back([map, key, function] {
            (void)map->computeIfAbsent(key, function);
        });
    }
    for (auto& thread : threads) thread.join();
    EXPECT_EQ(1, map->size());
    EXPECT_TRUE(map->get(::jxx::CAST<::jxx::lang::Object>(key))->utf8() == "computed");
}

TEST(ConcurrentHashMapParityTest, RejectsNullKeysAndValues) {
    const auto map = ::jxx::NEW<MapType>();
    const ::jxx::Ptr<::jxx::lang::Object> nullObject;
    const ::jxx::Ptr<S> nullValue;
    const auto key = ::jxx::NEW<S>("key");
    EXPECT_THROW(map->get(nullObject), ::jxx::lang::NullPointerException);
    EXPECT_THROW(map->put(key, nullValue), ::jxx::lang::NullPointerException);
    EXPECT_THROW(map->remove(nullObject), ::jxx::lang::NullPointerException);
}

TEST(ConcurrentMapParityTest, ConcreteMapIsUsableThroughInterface) {
    const auto concrete = ::jxx::NEW<MapType>();
    const auto map = ::jxx::CAST<::jxx::util::concurrent::ConcurrentMap<S, S>>(concrete);
    const auto key = ::jxx::NEW<S>("key");
    const auto value = ::jxx::NEW<S>("value");
    EXPECT_EQ(nullptr, map->putIfAbsent(key, value));
    EXPECT_TRUE(map->replace(key, value, ::jxx::NEW<S>("replacement")));
}

} // namespace
