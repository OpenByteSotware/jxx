#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <optional>
#include <sstream>
#include <thread>

#include "jxx.h"

using jxx::util::Dictionary;
using jxx::util::Map;
using jxx::util::Properties;

template <typename T>
static inline bool optEq(const std::optional<T>& a, const std::optional<T>& b) {
    return a.has_value() == b.has_value() && (!a.has_value() || *a == *b);
}

TEST(Properties_Defaults, LookupAndOverride) {
    Properties defaults;
    defaults.setProperty("host", "localhost");
    defaults.setProperty("port", "8080");
    defaults.setProperty("lang", "en");
    defaults.setProperty("arrow", "\xE2\x86\x92"); // UTF-8 for U+2192 (→), just for later checking

    Properties props(&defaults);
    props.setProperty("user", "james");
    props.setProperty("port", "9090"); // override default

    // Direct lookups
    EXPECT_EQ(props.getProperty("host"), "localhost"); // from defaults
    EXPECT_EQ(props.getProperty("port"), "9090");      // overridden
    EXPECT_EQ(props.getProperty("user"), "james");     // from self
    EXPECT_EQ(props.getProperty("missing"), "");       // no default => empty string
    EXPECT_EQ(props.getProperty("missing", "release"), "release");

    // Optional form
    auto u = props.getPropertyOpt("user");
    ASSERT_TRUE(u.has_value());
    EXPECT_EQ(*u, "james");

    auto m = props.getPropertyOpt("missing");
    EXPECT_FALSE(m.has_value());

    // propertyNames: union across this + defaults
    auto names = props.propertyNames();
    std::unordered_set<std::string> nameSet(names.begin(), names.end());
    EXPECT_TRUE(nameSet.count("host"));
    EXPECT_TRUE(nameSet.count("port"));
    EXPECT_TRUE(nameSet.count("user"));
    EXPECT_TRUE(nameSet.count("lang"));
    EXPECT_TRUE(nameSet.count("arrow"));
}

TEST(Properties_SetProperty, PreviousValueIsReturned) {
    Properties p;
    auto prev1 = p.setProperty("name", "Test");
    EXPECT_FALSE(prev1.has_value());

    auto prev2 = p.setProperty("name", "NewTest");
    ASSERT_TRUE(prev2.has_value());
    EXPECT_EQ(*prev2, "Test");

    // Through Hashtable/Map interface get()
    Map<std::string, std::string>* m = &p;
    auto got = m->get("name");
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, "NewTest");
}

TEST(Properties_StringPropertyNames, UnionAndUniqueness) {
    Properties defaults;
    defaults.setProperty("a", "1");
    defaults.setProperty("b", "2");

    Properties p(&defaults);
    p.setProperty("b", "22"); // shadow
    p.setProperty("c", "3");

    auto set = p.stringPropertyNames();
    EXPECT_EQ(set.count("a"), 1u);
    EXPECT_EQ(set.count("b"), 1u);
    EXPECT_EQ(set.count("c"), 1u);

    auto vec = p.stringPropertyNamesVector();
    std::unordered_set<std::string> vset(vec.begin(), vec.end());
    EXPECT_EQ(vset.size(), 3u);
    EXPECT_TRUE(vset.count("a"));
    EXPECT_TRUE(vset.count("b"));
    EXPECT_TRUE(vset.count("c"));
}

TEST(Properties_IO, LoadParsesCommentsSeparatorsEscapesContinuations) {
    // Test input exercising:
    // - Comments (#, !)
    // - Separators (=, :, and whitespace)
    // - Escaped key char ':' and escaped spaces in value
    // - Unicode escape \u2192 and newline \n
    // - Line continuation with trailing backslash
    std::istringstream in(
        "# Comment line\n"
        "! Another comment\n"
        "simple=value\n"
        "wssep   value2\n"                // whitespace separator
        "k=v with spaces\n"
        "path\\:key = val\\ with\\ spaces\n"
        "arrow=This\\npoints\\ to ? Target\n"
        "long = part1 \\\n"
        "  part2 \\\n"
        "  part3\n"
        "trailing=ends with backslash\\\\\n" // a literal backslash at end (escaped)
    );

    Properties p;
    p.load(in);

    EXPECT_EQ(p.getProperty("simple"), "value");
    EXPECT_EQ(p.getProperty("wssep"), "value2");
    EXPECT_EQ(p.getProperty("k"), "v with spaces");
    EXPECT_EQ(p.getProperty("path:key"), "val with spaces");
    EXPECT_EQ(p.getProperty("arrow"), "This\npoints to ? Target");
    EXPECT_EQ(p.getProperty("long"), "part1 part2 part3");
    EXPECT_EQ(p.getProperty("trailing"), "ends with backslash\\");
}

TEST(Properties_IO, StoreEscapesAndRoundTrips) {
    Properties p;
    p.setProperty(" name", " leading space");  // leading spaces on both sides
    p.setProperty("path:key", "val with\nnew line");
    p.setProperty("unicode", "\n?");

    // Store with a comment header; we won't assert date/header format exactly.
    std::ostringstream out;
    p.store(out, "App configuration\nSecond line");

    const std::string s = out.str();

    // Should contain escaped keys/values and unicode as \uXXXX; order is unspecified.
    // Check presence of expected escaped fragments.
    EXPECT_NE(s.find("#App configuration"), std::string::npos);
    EXPECT_NE(s.find("path\\:key=val with\\nnew line"), std::string::npos);
    EXPECT_NE(s.find("unicode=\\n?"), std::string::npos) // no guarantee of leading space-specific ordering
        << "Expected \\u2192 escape for non-ASCII";

    // Round trip: load back and compare values
    std::istringstream in(s);
    Properties q;
    q.load(in);

    EXPECT_EQ(q.getProperty(" name"), " leading space");
    EXPECT_EQ(q.getProperty("path:key"), "val with\nnew line");
    EXPECT_EQ(q.getProperty("unicode"), "?");
}

TEST(Properties_List, EmitsHumanReadableDumpWithDefaults) {
    Properties defaults;
    defaults.setProperty("a", "1");

    Properties p(&defaults);
    p.setProperty("b", "2");

    std::ostringstream oss;
    p.list(oss);
    const auto out = oss.str();

    // Must include current props and a marker for defaults; order not guaranteed.
    EXPECT_NE(out.find("b=2"), std::string::npos);
    EXPECT_NE(out.find("-- Defaults --"), std::string::npos);
    EXPECT_NE(out.find("a=1"), std::string::npos);
}

TEST(Properties_Threading, ConcurrentSetAndGetAreSynchronized) {
    Properties p;

    const int threads = 6;
    const int perThread = 500;

    std::vector<std::thread> workers;
    for (int t = 0; t < threads; ++t) {
        workers.emplace_back([&, t] {
            for (int i = 0; i < perThread; ++i) {
                const std::string key = "k" + std::to_string(t) + "_" + std::to_string(i);
                const std::string val = "v" + std::to_string(i);
                p.setProperty(key, val);
                auto got = p.getPropertyOpt(key);
                ASSERT_TRUE(got.has_value());
                ASSERT_EQ(*got, val);
            }
            });
    }
    for (auto& th : workers) th.join();

    // No deletes, so size should match.
    Map<std::string, std::string>* m = &p;
    EXPECT_EQ(m->size(), static_cast<std::size_t>(threads * perThread));
}

TEST(Properties_MapInterface, NullValuesForbiddenLikeHashtable) {
    Properties p;
    Map<std::string, std::string>* m = &p;

    // Putting a null value through the Map interface must throw (Hashtable semantics).
    EXPECT_THROW(m->put("bad", std::optional<std::string>{}), std::invalid_argument);

    // Valid put works
    auto prev = m->put("ok", std::optional<std::string>{"v"});
    EXPECT_FALSE(prev.has_value());
    auto got = m->get("ok");
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, "v");
}

TEST(Properties_DictionaryViews, KeysAndElementsSnapshots) {
    Properties p;
    p.setProperty("x", "1");
    p.setProperty("y", "2");

    Dictionary<std::string, std::string>* d = &p;
    auto keys = d->keys();
    auto elems = d->elements();

    ASSERT_EQ(keys.size(), 2u);
    ASSERT_EQ(elems.size(), 2u);

    // Mutate after snapshot; snapshots remain unchanged
    p.setProperty("z", "3");
    EXPECT_EQ(keys.size(), 2u);
    EXPECT_EQ(elems.size(), 2u);
}
