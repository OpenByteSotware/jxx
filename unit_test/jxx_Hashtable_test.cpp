#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <optional>
#include <thread>
#include <atomic>
#include <unordered_set>
#include "jxx.h"

using jxx::util::Dictionary;
using jxx::util::Map;
using jxx::util::Hashtable;

template <typename T>
static inline bool optEq(const std::optional<T>& a, const std::optional<T>& b) {
    return a.has_value() == b.has_value() && (!a.has_value() || *a == *b);
}

TEST(Hashtable_Basics, ConstructSizeAndClear) {
    Hashtable<std::string, int> ht;
    Map<std::string, int>* m = &ht;
    Dictionary<std::string, int>* d = &ht;

    EXPECT_TRUE(m->isEmpty());
    EXPECT_EQ(m->size(), 0u);
    EXPECT_TRUE(d->keys().empty());
    EXPECT_TRUE(d->elements().empty());

    m->put("a", 1);
    m->put("b", 2);
    EXPECT_FALSE(m->isEmpty());
    EXPECT_EQ(m->size(), 2u);

    m->clear();
    EXPECT_TRUE(m->isEmpty());
    EXPECT_EQ(m->size(), 0u);
    EXPECT_TRUE(d->keys().empty());
}

TEST(Hashtable_Crud, PutGetRemove_NoNullsAllowed) {
    Hashtable<std::string, int> ht;
    Map<std::string, int>* m = &ht;

    // put returns previous or null
    auto prevA = m->put("alpha", std::optional<int>{1});
    EXPECT_FALSE(prevA.has_value());
    EXPECT_TRUE(optEq(m->get("alpha"), std::optional<int>{1}));

    // overwrite returns previous
    auto prevA2 = m->put("alpha", std::optional<int>{3});
    ASSERT_TRUE(prevA2.has_value());
    EXPECT_EQ(*prevA2, 1);
    EXPECT_TRUE(optEq(m->get("alpha"), std::optional<int>{3}));

    // remove returns previous (or null)
    auto removed = m->remove("alpha");
    ASSERT_TRUE(removed.has_value());
    EXPECT_EQ(*removed, 3);
    EXPECT_FALSE(m->containsKey("alpha"));

    auto removedAbsent = m->remove("alpha");
    EXPECT_FALSE(removedAbsent.has_value());

    // Null values are forbidden (Hashtable semantics)
    EXPECT_THROW(m->put("n", std::optional<int>{}), std::invalid_argument);
}

TEST(Hashtable_Contains, ContainsKeyContainsValueAndLegacyContains) {
    Hashtable<std::string, int> ht;
    Map<std::string, int>* m = &ht;

    m->put("a", std::optional<int>{1});
    m->put("b", std::optional<int>{2});
    EXPECT_TRUE(m->containsKey("a"));
    EXPECT_FALSE(m->containsKey("z"));

    EXPECT_TRUE(m->containsValue(std::optional<int>{1}));
    EXPECT_FALSE(m->containsValue(std::optional<int>{3}));
    EXPECT_FALSE(m->containsValue(std::optional<int>{})); // null not stored in Hashtable

    // Legacy alias via Hashtable::contains
    EXPECT_TRUE(ht.contains(std::optional<int>{2}));
    EXPECT_FALSE(ht.contains(std::optional<int>{}));
}

TEST(Hashtable_Views, KeySetValuesEntrySetAndDictionaryViewsAreSnapshots) {
    Hashtable<std::string, int> ht;
    Map<std::string, int>* m = &ht;
    Dictionary<std::string, int>* d = &ht;

    m->put("x", 10);
    m->put("y", 20);

    auto keysSnapshot = m->keySet();
    auto valuesSnapshot = m->values();
    auto entriesSnapshot = m->entrySet();

    // Sizes
    ASSERT_EQ(keysSnapshot.size(), 2u);
    ASSERT_EQ(valuesSnapshot.size(), 2u);
    ASSERT_EQ(entriesSnapshot.size(), 2u);

    // Dictionary snapshots
    auto dkeys = d->keys();
    auto delems = d->elements();
    ASSERT_EQ(dkeys.size(), 2u);
    ASSERT_EQ(delems.size(), 2u);

    // Modify the map after snapshot; snapshots remain unchanged
    m->put("z", 30);
    EXPECT_EQ(keysSnapshot.size(), 2u);
    EXPECT_EQ(valuesSnapshot.size(), 2u);
    EXPECT_EQ(entriesSnapshot.size(), 2u);
    EXPECT_EQ(dkeys.size(), 2u);
    EXPECT_EQ(delems.size(), 2u);

    // Entry snapshots don't mutate the map
    auto e0 = entriesSnapshot[0];
    auto prev = e0.setValue(std::optional<int>{999}); // changes the snapshot only
    // Map values unchanged
    auto now = m->get(e0.getKey());
    EXPECT_FALSE(optEq(now, e0.getValue())); // snapshot differs from map
}

TEST(Hashtable_Defaults, GetOrDefaultPutIfAbsentReplaceRemoveKV) {
    Hashtable<std::string, int> ht;
    Map<std::string, int>* m = &ht;

    // getOrDefault when absent
    auto dflt = m->getOrDefault("none", std::optional<int>{7});
    EXPECT_TRUE(optEq(dflt, std::optional<int>{7}));

    // putIfAbsent
    auto r1 = m->putIfAbsent("q", std::optional<int>{1});
    EXPECT_FALSE(r1.has_value());
    EXPECT_TRUE(optEq(m->get("q"), std::optional<int>{1}));

    // Not absent → return current without changing
    auto r2 = m->putIfAbsent("q", std::optional<int>{2});
    EXPECT_TRUE(optEq(r2, std::optional<int>{1}));
    EXPECT_TRUE(optEq(m->get("q"), std::optional<int>{1}));

    // replace(k, v)
    auto r3 = m->replace("q", std::optional<int>{3});
    EXPECT_TRUE(optEq(r3, std::optional<int>{1}));
    EXPECT_TRUE(optEq(m->get("q"), std::optional<int>{3}));

    // replace(k, oldV, newV)
    bool ok1 = m->replace("q", std::optional<int>{1}, std::optional<int>{9});
    EXPECT_FALSE(ok1);
    bool ok2 = m->replace("q", std::optional<int>{3}, std::optional<int>{9});
    EXPECT_TRUE(ok2);
    EXPECT_TRUE(optEq(m->get("q"), std::optional<int>{9}));

    // remove(k, v)
    bool remNo = m->remove("q", std::optional<int>{3}); // mismatch
    EXPECT_FALSE(remNo);
    bool remYes = m->remove("q", std::optional<int>{9});
    EXPECT_TRUE(remYes);
    EXPECT_FALSE(m->containsKey("q"));

    // Null values are forbidden in replace/putIfAbsent
    EXPECT_THROW(m->replace("absent", std::optional<int>{}), std::invalid_argument);
    EXPECT_THROW(m->putIfAbsent("new", std::optional<int>{}), std::invalid_argument);
}

TEST(Hashtable_ComputeFamily, ComputeIfAbsentPresentComputeMerge) {
    Hashtable<std::string, int> ht;
    Map<std::string, int>* m = &ht;

    // computeIfAbsent: absent → compute; if null → no insert
    auto a1 = m->computeIfAbsent("a",
        [](const std::string&) { return std::optional<int>{10}; }
    );
    EXPECT_TRUE(optEq(a1, std::optional<int>{10}));
    EXPECT_TRUE(optEq(m->get("a"), std::optional<int>{10}));

    auto a2 = m->computeIfAbsent("b",
        [](const std::string&) { return std::optional<int>{}; } // null → no insert
    );
    EXPECT_FALSE(a2.has_value());
    EXPECT_FALSE(m->containsKey("b"));

    // computeIfPresent: only if present; null result → remove
    auto c1 = m->computeIfPresent("a",
        [](const std::string&, const std::optional<int>& cur) -> std::optional<int> {
            if (cur) return std::optional<int>{*cur + 1};
            return std::optional<int>{}; // would remove if invoked
        }
    );
    EXPECT_TRUE(optEq(c1, std::optional<int>{11}));
    EXPECT_TRUE(optEq(m->get("a"), std::optional<int>{11}));

    auto c2 = m->computeIfPresent("missing",
        [](const std::string&, const std::optional<int>&) {
            return std::optional<int>{42};
        }
    );
    EXPECT_FALSE(c2.has_value()); // not present

    // compute: always called; null result removes mapping if present
    auto d1 = m->compute("a",
        [](const std::string&, const std::optional<int>&) { return std::optional<int>{}; }
    );
    EXPECT_FALSE(d1.has_value());
    EXPECT_FALSE(m->containsKey("a"));

    auto d2 = m->compute("x",
        [](const std::string&, const std::optional<int>& cur) {
            // absent → cur == null; create 5
            (void)cur;
            return std::optional<int>{5};
        }
    );
    EXPECT_TRUE(optEq(d2, std::optional<int>{5}));
    EXPECT_TRUE(optEq(m->get("x"), std::optional<int>{5}));

    // merge: if absent, put value; else apply remapper; null result → remove
    auto mg1 = m->merge("x", std::optional<int>{10},
        [](const std::optional<int>& existing, const std::optional<int>& value) {
            return std::optional<int>{*existing + *value};
        });
    EXPECT_TRUE(optEq(mg1, std::optional<int>{15}));
    EXPECT_TRUE(optEq(m->get("x"), std::optional<int>{15}));

    auto mg2 = m->merge("x", std::optional<int>{1},
        [](const std::optional<int>&, const std::optional<int>&) {
            return std::optional<int>{}; // remove
        });
    EXPECT_FALSE(mg2.has_value());
    EXPECT_FALSE(m->containsKey("x"));

    // merge with null value is forbidden for Hashtable
    EXPECT_THROW(m->merge("y", std::optional<int>{},
        [](const std::optional<int>&, const std::optional<int>&) {
            return std::optional<int>{1};
        }), std::invalid_argument);
}

TEST(Hashtable_Copying, PutAllCopiesFromMapEntrySet) {
    Hashtable<std::string, int> htA;
    Hashtable<std::string, int> htB;

    Map<std::string, int>* mA = &htA;
    Map<std::string, int>* mB = &htB;

    mA->put("a", 1);
    mA->put("b", 2);

    // Use Hashtable's synchronized putAll
    htB.putAll(htA);

    EXPECT_TRUE(optEq(mB->get("a"), std::optional<int>{1}));
    EXPECT_TRUE(optEq(mB->get("b"), std::optional<int>{2}));
    EXPECT_EQ(mB->size(), 2u);
}

TEST(Hashtable_Capacity, LoadFactorAndReserve) {
    Hashtable<std::string, int> ht;

    // basic sanity — operations shouldn't throw
    ht.setLoadFactor(0.5f);
    EXPECT_GT(ht.loadFactor(), 0.0f);

    ht.ensureCapacity(1000); // reserve
    // No direct way to assert capacity; just ensure no throw and still usable
    auto prev = ht.put("k", 1);
    EXPECT_FALSE(prev.has_value());
    EXPECT_TRUE(optEq(ht.get("k"), std::optional<int>{1}));

    // Invalid load factor
    EXPECT_THROW(ht.setLoadFactor(0.0f), std::invalid_argument);
    EXPECT_THROW(ht.setLoadFactor(-1.0f), std::invalid_argument);
}

TEST(Hashtable_Threading, ConcurrentPutsAndGetsAreSynchronized) {
    Hashtable<std::string, int> ht;
    Map<std::string, int>* m = &ht;

    const int threads = 8;
    const int perThread = 1000;

    std::vector<std::thread> workers;
    workers.reserve(threads);

    // Each thread writes a disjoint range of keys and performs reads.
    for (int t = 0; t < threads; ++t) {
        workers.emplace_back([&, t] {
            for (int i = 0; i < perThread; ++i) {
                std::string key = "k" + std::to_string(t) + "_" + std::to_string(i);
                m->put(key, std::optional<int>{i});
                auto v = m->get(key);
                ASSERT_TRUE(v.has_value());
                ASSERT_EQ(*v, i);
            }
            });
    }
    for (auto& th : workers) th.join();

    // Verify size (best-effort; in Hashtable there are no deletes in this test).
    EXPECT_EQ(m->size(), static_cast<std::size_t>(threads * perThread));
}