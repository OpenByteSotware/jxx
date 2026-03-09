#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include "jxx.h" 

using jxx::util::Map;
using jxx::util::TreeMap;
using namespace std;

// A small helper for readability
template <typename T>
static inline bool optEq(const std::optional<T>& a, const std::optional<T>& b) {
    return a.has_value() == b.has_value() && (!a.has_value() || *a == *b);
}

TEST(TreeMap_Basics, EmptyOnConstruct) {
    TreeMap<std::string, int> tm;
    Map<std::string, int>* m = &tm;

    EXPECT_TRUE(m->isEmpty());
    EXPECT_TRUE(m->empty());
    EXPECT_EQ(m->size(), 0u);

    // firstKey/lastKey should throw on empty
    EXPECT_THROW(tm.firstKey(), std::out_of_range);
    EXPECT_THROW(tm.lastKey(), std::out_of_range);
}

TEST(TreeMap_Crud, PutGetRemove_WithNullValues) {
    TreeMap<std::string, int> tm;
    Map<std::string, int>* m = &tm;

    // put returns previous (null for new)
    auto prevA = m->put("alpha", 1);
    EXPECT_FALSE(prevA.has_value());
    EXPECT_EQ(*m->get("alpha"), 1);

    // Put null value (present-null)
    auto prevB = m->put("bravo", std::nullopt);
    EXPECT_FALSE(prevB.has_value());

    // get returns nullopt for BOTH absent and present-null (Java ambiguity)
    auto vBravo = m->get("bravo");
    EXPECT_FALSE(vBravo.has_value());      // present-null
    auto vZed = m->get("zed");
    EXPECT_FALSE(vZed.has_value());        // absent

    // Overwrite existing value
    auto prevA2 = m->put("alpha", 3);
    ASSERT_TRUE(prevA2.has_value());
    EXPECT_EQ(*prevA2, 1);
    EXPECT_EQ(*m->get("alpha"), 3);

    // remove returns previous value (or null)
    auto removedAlpha = m->remove("alpha");
    ASSERT_TRUE(removedAlpha.has_value());
    EXPECT_EQ(*removedAlpha, 3);
    EXPECT_FALSE(m->containsKey("alpha"));

    // remove absent returns null
    auto removedNo = m->remove("missing");
    EXPECT_FALSE(removedNo.has_value());
}

TEST(TreeMap_Contains, ContainsKeyContainsValue) {
    TreeMap<std::string, int> tm;
    Map<std::string, int>* m = &tm;

    m->put("a", 1);
    m->put("b", std::nullopt);
    m->put("c", 3);

    EXPECT_TRUE(m->containsKey("a"));
    EXPECT_TRUE(m->containsKey("b"));
    EXPECT_FALSE(m->containsKey("z"));

    // containsValue runs O(n). We expect it to match std::optional equality.
    EXPECT_TRUE(m->containsValue(std::optional<int>{1}));
    EXPECT_TRUE(m->containsValue(std::optional<int>{})); // present-null exists at "b"
    EXPECT_FALSE(m->containsValue(std::optional<int>{2}));
}

TEST(TreeMap_Navigation, FirstLastAndNeighbors) {
    TreeMap<std::string, int> tm;
    Map<std::string, int>* m = &tm;

    m->put("alpha", 1);
    m->put("bravo", 2);
    m->put("charlie", 3);

    EXPECT_EQ(tm.firstKey(), "alpha");
    EXPECT_EQ(tm.lastKey(), "charlie");

    // lowerKey: greatest key < k
    EXPECT_EQ(tm.lowerKey("bravo"), std::optional<std::string>{"alpha"});
    EXPECT_EQ(tm.lowerKey("alpha"), std::nullopt);

    // floorKey: greatest key <= k
    EXPECT_EQ(tm.floorKey("bravo"), std::optional<std::string>{"bravo"});
    EXPECT_EQ(tm.floorKey("aardvark"), std::nullopt);

    // ceilingKey: smallest key >= k
    EXPECT_EQ(tm.ceilingKey("beta"), std::optional<std::string>{"bravo"});
    EXPECT_EQ(tm.ceilingKey("charlie"), std::optional<std::string>{"charlie"});
    EXPECT_EQ(tm.ceilingKey("zzz"), std::nullopt);

    // higherKey: smallest key > k
    EXPECT_EQ(tm.higherKey("bravo"), std::optional<std::string>{"charlie"});
    EXPECT_EQ(tm.higherKey("charlie"), std::nullopt);
}

TEST(TreeMap_Views, KeySetValuesEntrySetMaterialized) {
    TreeMap<std::string, int> tm;
    Map<std::string, int>* m = &tm;

    m->put("bravo", 2);
    m->put("alpha", 1);
    m->put("charlie", std::nullopt);

    // Keys should be sorted ascending
    auto keys = m->keySet();
    ASSERT_EQ(keys.size(), 3u);
    EXPECT_EQ(keys[0], "alpha");
    EXPECT_EQ(keys[1], "bravo");
    EXPECT_EQ(keys[2], "charlie");

    // Values reflect current mapping order (by key)
    auto vals = m->values();
    ASSERT_EQ(vals.size(), 3u);
    ASSERT_TRUE(vals[0].has_value());
    EXPECT_EQ(*vals[0], 1);
    ASSERT_TRUE(vals[1].has_value());
    EXPECT_EQ(*vals[1], 2);
    EXPECT_FALSE(vals[2].has_value()); // null

    // Entry set mirrors
    auto entries = m->entrySet();
    ASSERT_EQ(entries.size(), 3u);
    //EXPECT_EQ(entries[0].first, "alpha");
    //EXPECT_TRUE(entries[0].second.has_value());
    //EXPECT_EQ(*entries[0].second, 1);

    // Entries/materialized vector is a snapshot — modifying map later won't mutate vector
    auto snap = m->entrySet();
    m->put("delta", 4);
    EXPECT_EQ(snap.size(), 3u);     // unchanged
}

TEST(Map_Defaults, GetOrDefaultPutIfAbsentReplaceRemoveKV) {
    TreeMap<std::string, int> tm;
    Map<std::string, int>* m = &tm;

    // getOrDefault: return default if key absent; if present (even null) return get(key)
    EXPECT_TRUE(optEq(m->getOrDefault("none", std::optional<int>{7}), std::optional<int>{7}));

    m->put("p", std::nullopt);
    EXPECT_FALSE(m->getOrDefault("p", std::optional<int>{9}).has_value()); // present-null → null

    // putIfAbsent: if absent, insert & return null; if present, return current without changing it
    auto r1 = m->putIfAbsent("q", 1);
    EXPECT_FALSE(r1.has_value());                      // previously absent
    EXPECT_TRUE(optEq(m->get("q"), std::optional<int>{1}));

    auto r2 = m->putIfAbsent("q", 2);
    EXPECT_TRUE(optEq(r2, std::optional<int>{1}));     // previous value returned
    EXPECT_TRUE(optEq(m->get("q"), std::optional<int>{1})); // unchanged

    // replace(k, v): only if present
    auto r3 = m->replace("q", 3);
    EXPECT_TRUE(optEq(r3, std::optional<int>{1}));
    EXPECT_TRUE(optEq(m->get("q"), std::optional<int>{3}));

    auto r4 = m->replace("absent", 42);
    EXPECT_FALSE(r4.has_value());                      // not present

    // replace(k, oldV, newV): conditional
    bool ok1 = m->replace("q", std::optional<int>{2}, 9);
    EXPECT_FALSE(ok1);
    bool ok2 = m->replace("q", std::optional<int>{3}, 9);
    EXPECT_TRUE(ok2);
    EXPECT_TRUE(optEq(m->get("q"), std::optional<int>{9}));

    // remove(k, v): conditional
    bool remNo = m->remove("q", std::optional<int>{3}); // current is 9
    EXPECT_FALSE(remNo);
    bool remYes = m->remove("q", std::optional<int>{9});
    EXPECT_TRUE(remYes);
    EXPECT_FALSE(m->containsKey("q"));
}

TEST(Map_ComputeFamily, ComputeIfAbsentPresentComputeMerge) {
    TreeMap<std::string, int> tm;
    Map<std::string, int>* m = &tm;

    // computeIfAbsent: absent → call; null result means "do not insert"
    auto a1 = m->computeIfAbsent("a", [](const std::string& k) {
        return std::optional<int>{10};
        });
    EXPECT_TRUE(optEq(a1, std::optional<int>{10}));
    EXPECT_TRUE(optEq(m->get("a"), std::optional<int>{10}));

    auto a2 = m->computeIfAbsent("b", [](const std::string& k) {
        return std::optional<int>{}; // null → no insert
        });
    EXPECT_FALSE(a2.has_value());
    EXPECT_FALSE(m->containsKey("b"));

    // computeIfPresent: only if present; null result → remove
    auto c1 = m->computeIfPresent("a", [](const std::string& k, const std::optional<int>& curr) {
        // increment if present
        if (curr) return std::optional<int>{*curr + 1};
        return std::optional<int>{}; // would remove if invoked
        });
    EXPECT_TRUE(optEq(c1, std::optional<int>{11}));
    EXPECT_TRUE(optEq(m->get("a"), std::optional<int>{11}));

    auto c2 = m->computeIfPresent("missing", [](const std::string&, const std::optional<int>&) {
        return std::optional<int>{42};
        });
    EXPECT_FALSE(c2.has_value()); // not present → no call, returns null

    // compute: always called; null result removes mapping if present
    auto d1 = m->compute("a", [](const std::string& k, const std::optional<int>& curr) {
        return std::optional<int>{}; // remove
        });
    EXPECT_FALSE(d1.has_value());
    EXPECT_FALSE(m->containsKey("a"));

    auto d2 = m->compute("x", [](const std::string& k, const std::optional<int>& curr) {
        // absent → curr == null; create 5
        return std::optional<int>{5};
        });
    EXPECT_TRUE(optEq(d2, std::optional<int>{5}));
    EXPECT_TRUE(optEq(m->get("x"), std::optional<int>{5}));

    // merge: if absent, put value (non-null only). If present, apply remapper.
    auto mg1 = m->merge("x", std::optional<int>{10},
        [](const std::optional<int>& existing, const std::optional<int>& value) {
            return std::optional<int>{*existing + *value};
        });
    EXPECT_TRUE(optEq(mg1, std::optional<int>{15}));
    EXPECT_TRUE(optEq(m->get("x"), std::optional<int>{15}));

    // merge with null result → remove key
    auto mg2 = m->merge("x", std::optional<int>{1},
        [](const std::optional<int>& existing, const std::optional<int>& value) {
            return std::optional<int>{}; // signals remove
        });
    EXPECT_FALSE(mg2.has_value());
    EXPECT_FALSE(m->containsKey("x"));

    // merge absent with null value → no-op (consistent with the provided Map::merge)
    auto mg3 = m->merge("y", std::optional<int>{}, [](auto&, auto&) {
        return std::optional<int>{999};
        });
    EXPECT_FALSE(mg3.has_value());
    EXPECT_FALSE(m->containsKey("y"));
}

struct CaseInsensitiveLess {
    bool operator()(const std::string& a, const std::string& b) const {
        const auto n = min(a.size(), b.size());
        for (std::size_t i = 0; i < n; ++i) {
            unsigned char ca = static_cast<unsigned char>(std::tolower(a[i]));
            unsigned char cb = static_cast<unsigned char>(std::tolower(b[i]));
            if (ca < cb) return true;
            if (ca > cb) return false;
        }
        return a.size() < b.size();
    }
};

TEST(TreeMap_Comparator, CaseInsensitiveOrdering) {
    TreeMap<std::string, int, CaseInsensitiveLess> tm;
    Map<std::string, int>* m = &tm;

    m->put("Alpha", 1);
    m->put("bravo", 2);
    m->put("CHARLIE", 3);

    // Ordering should be case-insensitive
    auto keys = m->keySet();
    ASSERT_EQ(keys.size(), 3u);
    EXPECT_EQ(keys[0], "Alpha");
    EXPECT_EQ(keys[1], "bravo");
    EXPECT_EQ(keys[2], "CHARLIE");

    // Navigational ops should honor comparator too
    EXPECT_EQ(tm.floorKey("bravo"), std::optional<std::string>{"bravo"});
    EXPECT_EQ(tm.ceilingKey("beta"), std::optional<std::string>{"bravo"});
}