#pragma once

#include "util/jxx.util.EmptyStackException.h"
#include "util/jxx.util.Vector.h"

namespace jxx::util {

template <typename E>
class Stack : public Vector<E> {
public:
    using JxxSuper = Vector<E>;
    using Super = Vector<E>;

    Stack() = default;
    ~Stack() override = default;

    ::jxx::Ptr<E> push(const ::jxx::Ptr<E>& item) {
        this->addElement(item);
        return item;
    }

    ::jxx::Ptr<E> pop() {
        if (this->isEmpty()) throw EmptyStackException();
        const auto index = this->size() - 1;
        auto value = this->elementAt(index);
        this->removeElementAt(index);
        return value;
    }

    ::jxx::Ptr<E> peek() {
        if (this->isEmpty()) throw EmptyStackException();
        return this->lastElement();
    }

    ::jxx::lang::jbool empty() {
        return this->isEmpty();
    }

    ::jxx::lang::jint search(
        const ::jxx::Ptr<::jxx::lang::Object>& object) {
        const auto index = this->lastIndexOf(object);
        return index < 0 ? -1 : this->size() - index;
    }
};

} // namespace jxx::util
