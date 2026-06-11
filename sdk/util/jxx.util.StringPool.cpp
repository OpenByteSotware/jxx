#include "util/jxx.util.StringPool.h"
#include "lang/jxx.lang.String.h"

#include <mutex>
#include <string>
#include <unordered_map>

namespace jxx::util {
namespace {
    using StringPtr = jxx::Ptr<jxx::lang::String>;

    std::unordered_map<std::string, StringPtr>& pool() {
        static std::unordered_map<std::string, StringPtr> instance;
        return instance;
    }

    std::mutex& poolMutex() {
        static std::mutex m;
        return m;
    }
}

jxx::Ptr<jxx::lang::String> StringPool::intern(const char* text) {
    if (text == nullptr) {
        return jxx::Ptr<jxx::lang::String>();
    }

    std::lock_guard<std::mutex> lock(poolMutex());
    auto& p = pool();
    auto it = p.find(text);
    if (it != p.end()) {
        return it->second;
    }

    auto s = jxx::lang::String::valueOf(text);
    p.emplace(text, s);
    return s;
}

} // namespace jxx::util
