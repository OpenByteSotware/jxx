#pragma once

#include <functional>
#include <vector>

namespace jxx::lang::thread_local_detail {

class InheritableEntry {
public:
    virtual ~InheritableEntry() = default;
    virtual std::function<void()> captureForChild() = 0;
};

void addInheritableEntry(InheritableEntry* entry);
void removeInheritableEntry(InheritableEntry* entry);
std::vector<std::function<void()>> captureInheritedValues();

} // namespace jxx::lang::thread_local_detail
