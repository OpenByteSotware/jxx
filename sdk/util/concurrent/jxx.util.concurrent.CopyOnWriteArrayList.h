#pragma once

#include <mutex>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.ArrayList.h"

namespace jxx::util::concurrent {

template<typename E>
class CopyOnWriteArrayList final
    : public ::jxx::lang::ClassBase<
          CopyOnWriteArrayList<E>,
          ::jxx::util::ArrayList<E>> {
private:
    class SnapshotIterator final
        : public ::jxx::lang::ClassBase<
              SnapshotIterator,
              ::jxx::lang::Object,
              ::jxx::util::Iterator<E>> {
    public:
        using Super = ::jxx::lang::ClassBase<
            SnapshotIterator,
            ::jxx::lang::Object,
            ::jxx::util::Iterator<E>>;

        explicit SnapshotIterator(std::vector<::jxx::Ptr<E>> values)
            : Super(), values_(std::move(values)) {}

        ::jxx::lang::jbool hasNext() override {
            return index_ < values_.size();
        }

        ::jxx::Ptr<E> next() override {
            if (!hasNext()) throw ::jxx::util::NoSuchElementException();
            return values_[index_++];
        }

        void remove() override {
            throw ::jxx::lang::UnsupportedOperationException();
        }

    private:
        std::vector<::jxx::Ptr<E>> values_;
        std::size_t index_ = 0;
    };

public:
    using JxxSuper = ::jxx::util::ArrayList<E>;
    using Super = ::jxx::lang::ClassBase<CopyOnWriteArrayList<E>, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    CopyOnWriteArrayList() : Super() {}

    ::jxx::lang::jint size() override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::size();
    }

    ::jxx::Ptr<E> get(::jxx::lang::jint index) const override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::get(index);
    }

    ::jxx::Ptr<E> set(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<E>& element) override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::set(index, element);
    }

    ::jxx::lang::jbool add(const ::jxx::Ptr<E>& element) override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::add(element);
    }

    void add(
        ::jxx::lang::jint index,
        const ::jxx::Ptr<E>& element) override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        JxxSuper::add(index, element);
    }

    ::jxx::Ptr<E> remove(::jxx::lang::jint index) override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::remove(index);
    }

    ::jxx::lang::jbool remove(
        const ::jxx::Ptr<::jxx::lang::Object>& object) override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return JxxSuper::remove(object);
    }

    void clear() override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        JxxSuper::clear();
    }

    ::jxx::lang::jbool addIfAbsent(const ::jxx::Ptr<E>& element) {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        auto object = ::jxx::CAST<::jxx::lang::Object>(element);
        if (JxxSuper::contains(object)) return false;
        return JxxSuper::add(element);
    }

    ::jxx::Ptr<::jxx::util::Iterator<E>> iterator() override {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        std::vector<::jxx::Ptr<E>> snapshot;
        snapshot.reserve(static_cast<std::size_t>(JxxSuper::size()));
        for (::jxx::lang::jint i = 0; i < JxxSuper::size(); ++i) {
            snapshot.push_back(JxxSuper::get(i));
        }
        return ::jxx::CAST<::jxx::util::Iterator<E>>(
            ::jxx::NEW<SnapshotIterator>(std::move(snapshot)));
    }

private:
    mutable std::recursive_mutex mutex_;
};

} // namespace jxx::util::concurrent
