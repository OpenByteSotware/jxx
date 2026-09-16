#pragma once

#include <functional>
#include <vector>

namespace jxx::lang::thread_local_detail {

class LocalEntry {
public:
    virtual ~LocalEntry() = default;
    virtual void removeCurrentThreadValue() = 0;
};

class InheritableEntry {
public:
    virtual ~InheritableEntry() = default;
    virtual std::function<void()> captureForChild() = 0;
};

void addLocalEntry(LocalEntry* entry);
void removeLocalEntry(LocalEntry* entry);
void clearCurrentThreadValues();
void addInheritableEntry(InheritableEntry* entry);
void removeInheritableEntry(InheritableEntry* entry);
std::vector<std::function<void()>> captureInheritedValues();

} // namespace jxx::lang::thread_local_detail
