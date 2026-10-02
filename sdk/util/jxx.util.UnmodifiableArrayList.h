#pragma once

#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.ArrayList.h"

namespace jxx::util {

template <typename E>
class UnmodifiableArrayList final
    : public ::jxx::lang::ClassBase<
          UnmodifiableArrayList<E>,
          ArrayList<E>> {
public:
    using JxxSuper = ArrayList<E>;
    using Super = ::jxx::lang::ClassBase<
        UnmodifiableArrayList<E>,
        JxxSuper>;

    UnmodifiableArrayList() = default;

    explicit UnmodifiableArrayList(
        const ::jxx::Ptr<List<E>>& source) {
        if (source != nullptr) {
            for (::jxx::lang::jint index = 0;
                 index < source->size(); ++index) {
                JxxSuper::add(source->get(index));
            }
        }
    }

    ::jxx::lang::jbool add(const ::jxx::Ptr<E>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    void add(::jxx::lang::jint, const ::jxx::Ptr<E>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::lang::jbool addAll(
        const ::jxx::Ptr<wildcard::CollectionExtends<E>>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::lang::jbool addAll(
        ::jxx::lang::jint,
        const ::jxx::Ptr<wildcard::CollectionExtends<E>>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::Ptr<E> set(
        ::jxx::lang::jint,
        const ::jxx::Ptr<E>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::Ptr<E> remove(::jxx::lang::jint) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::lang::jbool remove(
        const ::jxx::Ptr<::jxx::lang::Object>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::lang::jbool removeAll(
        const ::jxx::Ptr<wildcard::CollectionAny>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::lang::jbool retainAll(
        const ::jxx::Ptr<wildcard::CollectionAny>&) override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    void clear() override {
        throw ::jxx::lang::UnsupportedOperationException();
    }
};

} // namespace jxx::util
