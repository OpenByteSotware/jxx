#pragma once
#include "util/jxx.util.Collection.h"
namespace jxx::util {
template <typename E>
class Queue : public virtual Collection<E> {
public:
    virtual ~Queue() = default;
    virtual jxx::lang::jbool offer(const jxx::Ptr<E>& element) = 0;
    virtual jxx::Ptr<E> remove() = 0;
    virtual jxx::Ptr<E> poll() = 0;
    virtual jxx::Ptr<E> element() = 0;
    virtual jxx::Ptr<E> peek() = 0;
};
} // namespace jxx::util
