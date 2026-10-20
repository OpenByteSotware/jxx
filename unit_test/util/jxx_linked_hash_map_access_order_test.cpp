#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.LinkedHashMap.h"
#include "util/jxx.util.MapEntry.h"

namespace {

class FiveEntryCache final
    : public ::jxx::util::LinkedHashMap<
          ::jxx::lang::Integer, ::jxx::lang::String> {
public:
    FiveEntryCache()
        : ::jxx::util::LinkedHashMap<
              ::jxx::lang::Integer, ::jxx::lang::String>(16, 0.75F, true) {}

protected:
    ::jxx::lang::jbool removeEldestEntry(
        const ::jxx::Ptr<::jxx::util::MapEntry<
            ::jxx::lang::Integer, ::jxx::lang::String>>&) override {
        return size() > 5;
    }
};

TEST(LinkedHashMapParity, AccessOrderEvictsLeastRecentlyAccessedEntry)
{
    auto cache = ::jxx::NEW<FiveEntryCache>();
    for (::jxx::lang::jint value = 1; value <= 5; ++value) {
        cache->put(::jxx::lang::Integer::valueOf(value),
            ::jxx::NEW<::jxx::lang::String>(std::to_string(value)));
    }
    ASSERT_NE(cache->get(::jxx::lang::Integer::valueOf(1)), nullptr);
    cache->put(::jxx::lang::Integer::valueOf(6),
        ::jxx::NEW<::jxx::lang::String>("6"));

    EXPECT_EQ(cache->size(), 5);
    EXPECT_TRUE(cache->containsKey(::jxx::lang::Integer::valueOf(1)));
    EXPECT_FALSE(cache->containsKey(::jxx::lang::Integer::valueOf(2)));
    EXPECT_TRUE(cache->containsKey(::jxx::lang::Integer::valueOf(6)));
}

TEST(LinkedHashMapParity, ContainsKeyDoesNotChangeAccessOrder)
{
    auto cache = ::jxx::NEW<FiveEntryCache>();
    for (::jxx::lang::jint value = 1; value <= 5; ++value) {
        cache->put(::jxx::lang::Integer::valueOf(value),
            ::jxx::NEW<::jxx::lang::String>(std::to_string(value)));
    }
    EXPECT_TRUE(cache->containsKey(::jxx::lang::Integer::valueOf(1)));
    cache->put(::jxx::lang::Integer::valueOf(6),
        ::jxx::NEW<::jxx::lang::String>("6"));
    EXPECT_FALSE(cache->containsKey(::jxx::lang::Integer::valueOf(1)));
}

} // namespace
