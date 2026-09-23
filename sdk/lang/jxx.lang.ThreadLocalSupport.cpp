#include <algorithm>
#include <mutex>
#include "lang/jxx.lang.ThreadLocalSupport.h"


namespace jxx::lang::thread_local_detail {
namespace {
std::mutex& registryMutex() { static std::mutex value; return value; }
std::vector<LocalEntry*>& localRegistry() { static std::vector<LocalEntry*> value; return value; }
std::vector<InheritableEntry*>& inheritableRegistry() { static std::vector<InheritableEntry*> value; return value; }
}
void addLocalEntry(LocalEntry* entry) {
    std::lock_guard<std::mutex> lock(registryMutex());
    localRegistry().push_back(entry);
}
void removeLocalEntry(LocalEntry* entry) {
    std::lock_guard<std::mutex> lock(registryMutex());
    auto& values = localRegistry();
    values.erase(std::remove(values.begin(), values.end(), entry), values.end());
}
void clearCurrentThreadValues() {
    std::vector<LocalEntry*> entries;
    {
        std::lock_guard<std::mutex> lock(registryMutex());
        entries = localRegistry();
    }
    for (auto* entry : entries) entry->removeCurrentThreadValue();
}
void addInheritableEntry(InheritableEntry* entry) {
    std::lock_guard<std::mutex> lock(registryMutex());
    inheritableRegistry().push_back(entry);
}
void removeInheritableEntry(InheritableEntry* entry) {
    std::lock_guard<std::mutex> lock(registryMutex());
    auto& values = inheritableRegistry();
    values.erase(std::remove(values.begin(), values.end(), entry), values.end());
}
std::vector<std::function<void()>> captureInheritedValues() {
    std::lock_guard<std::mutex> lock(registryMutex());
    std::vector<std::function<void()>> result;
    for (auto* entry : inheritableRegistry()) {
        auto action = entry->captureForChild();
        if (action) result.push_back(std::move(action));
    }
    return result;
}
} // namespace jxx::lang::thread_local_detail
