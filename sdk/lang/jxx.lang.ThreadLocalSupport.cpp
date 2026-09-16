#include "lang/jxx.lang.ThreadLocalSupport.h"

#include <algorithm>
#include <mutex>

namespace jxx::lang::thread_local_detail {
namespace {
std::mutex& registryMutex() { static std::mutex value; return value; }
std::vector<InheritableEntry*>& registry() { static std::vector<InheritableEntry*> value; return value; }
}
void addInheritableEntry(InheritableEntry* entry) {
    std::lock_guard<std::mutex> lock(registryMutex());
    registry().push_back(entry);
}
void removeInheritableEntry(InheritableEntry* entry) {
    std::lock_guard<std::mutex> lock(registryMutex());
    auto& values = registry();
    values.erase(std::remove(values.begin(), values.end(), entry), values.end());
}
std::vector<std::function<void()>> captureInheritedValues() {
    std::lock_guard<std::mutex> lock(registryMutex());
    std::vector<std::function<void()>> result;
    for (auto* entry : registry()) {
        auto action = entry->captureForChild();
        if (action) result.push_back(std::move(action));
    }
    return result;
}
} // namespace jxx::lang::thread_local_detail
