#pragma once

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.Set.h"

namespace jxx::com::sun::net::httpserver::internal {

template <typename E>
class ReadOnlyIterator final
    : public ::jxx::lang::ClassBase<
          ReadOnlyIterator<E>,
          ::jxx::lang::Object,
          ::jxx::util::Iterator<E>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ReadOnlyIterator<E>, JxxSuper, ::jxx::util::Iterator<E>>;

    explicit ReadOnlyIterator(
        const ::jxx::Ptr<::jxx::util::Iterator<E>>& delegate)
        : Super(), delegate_(delegate)
    {
        if (delegate_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    ::jxx::lang::jbool hasNext() override { return delegate_->hasNext(); }
    ::jxx::Ptr<E> next() override { return delegate_->next(); }
    void remove() override { throw ::jxx::lang::UnsupportedOperationException(); }

private:
    ::jxx::Ptr<::jxx::util::Iterator<E>> delegate_;
};

template <typename K, typename V>
class ReadOnlyMapEntry final
    : public ::jxx::lang::ClassBase<
          ReadOnlyMapEntry<K, V>,
          ::jxx::lang::Object,
          ::jxx::util::MapEntry<K, V>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ReadOnlyMapEntry<K, V>, JxxSuper, ::jxx::util::MapEntry<K, V>>;

    explicit ReadOnlyMapEntry(
        const ::jxx::Ptr<::jxx::util::MapEntry<K, V>>& delegate)
        : Super(), delegate_(delegate)
    {
        if (delegate_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    ::jxx::Ptr<K> getKey() override { return delegate_->getKey(); }
    ::jxx::Ptr<V> getValue() override { return delegate_->getValue(); }
    ::jxx::Ptr<V> setValue(const ::jxx::Ptr<V>&) override
    {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const override
    {
        return delegate_->equals(object);
    }
    ::jxx::lang::jint hashCode() const override { return delegate_->hashCode(); }

private:
    ::jxx::Ptr<::jxx::util::MapEntry<K, V>> delegate_;
};

template <typename K, typename V>
class ReadOnlyEntryIterator final
    : public ::jxx::lang::ClassBase<
          ReadOnlyEntryIterator<K, V>,
          ::jxx::lang::Object,
          ::jxx::util::Iterator<::jxx::util::MapEntry<K, V>>> {
public:
    using Entry = ::jxx::util::MapEntry<K, V>;
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ReadOnlyEntryIterator<K, V>, JxxSuper, ::jxx::util::Iterator<Entry>>;

    explicit ReadOnlyEntryIterator(
        const ::jxx::Ptr<::jxx::util::Iterator<Entry>>& delegate)
        : Super(), delegate_(delegate)
    {
        if (delegate_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    ::jxx::lang::jbool hasNext() override { return delegate_->hasNext(); }
    ::jxx::Ptr<Entry> next() override
    {
        return ::jxx::CAST<Entry>(
            ::jxx::NEW<ReadOnlyMapEntry<K, V>>(delegate_->next()));
    }
    void remove() override { throw ::jxx::lang::UnsupportedOperationException(); }

private:
    ::jxx::Ptr<::jxx::util::Iterator<Entry>> delegate_;
};

template <typename E>
class ReadOnlyCollection final
    : public ::jxx::lang::ClassBase<
          ReadOnlyCollection<E>,
          ::jxx::lang::Object,
          ::jxx::util::Collection<E>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ReadOnlyCollection<E>, JxxSuper, ::jxx::util::Collection<E>>;

    explicit ReadOnlyCollection(
        const ::jxx::Ptr<::jxx::util::Collection<E>>& delegate)
        : Super(), delegate_(delegate)
    {
        if (delegate_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    ::jxx::lang::jint size() override { return delegate_->size(); }
    ::jxx::lang::jbool isEmpty() override { return delegate_->isEmpty(); }
    ::jxx::lang::jbool contains(const ::jxx::Ptr<::jxx::lang::Object>& value) override { return delegate_->contains(value); }
    ::jxx::Ptr<::jxx::util::Iterator<E>> iterator() override
    {
        return ::jxx::CAST<::jxx::util::Iterator<E>>(
            ::jxx::NEW<ReadOnlyIterator<E>>(delegate_->iterator()));
    }
    ::jxx::lang::ObjectArray toArray() override { return delegate_->toArray(); }
    ::jxx::lang::jbool containsAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionAny>& values) override { return delegate_->containsAll(values); }
    ::jxx::lang::jbool add(const ::jxx::Ptr<E>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool remove(const ::jxx::Ptr<::jxx::lang::Object>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool addAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionExtends<E>>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool removeAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionAny>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool retainAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionAny>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    void clear() override { throw ::jxx::lang::UnsupportedOperationException(); }

private:
    ::jxx::Ptr<::jxx::util::Collection<E>> delegate_;
};

template <typename E>
class ReadOnlySet final
    : public ::jxx::lang::ClassBase<
          ReadOnlySet<E>,
          ::jxx::lang::Object,
          ::jxx::util::Set<E>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<ReadOnlySet<E>, JxxSuper, ::jxx::util::Set<E>>;

    explicit ReadOnlySet(const ::jxx::Ptr<::jxx::util::Set<E>>& delegate)
        : Super(), delegate_(delegate)
    {
        if (delegate_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    ::jxx::lang::jint size() override { return delegate_->size(); }
    ::jxx::lang::jbool isEmpty() override { return delegate_->isEmpty(); }
    ::jxx::lang::jbool contains(const ::jxx::Ptr<::jxx::lang::Object>& value) override { return delegate_->contains(value); }
    ::jxx::Ptr<::jxx::util::Iterator<E>> iterator() override
    {
        return ::jxx::CAST<::jxx::util::Iterator<E>>(
            ::jxx::NEW<ReadOnlyIterator<E>>(delegate_->iterator()));
    }
    ::jxx::lang::ObjectArray toArray() override { return delegate_->toArray(); }
    ::jxx::lang::jbool containsAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionAny>& values) override { return delegate_->containsAll(values); }
    ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override { return delegate_->equals(object); }
    ::jxx::lang::jint hashCode() const override { return delegate_->hashCode(); }
    ::jxx::Ptr<::jxx::util::Spliterator<E>> spliterator() override
    {
        return delegate_->spliterator();
    }
    ::jxx::lang::jbool add(const ::jxx::Ptr<E>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool remove(const ::jxx::Ptr<::jxx::lang::Object>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool addAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionExtends<E>>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool removeAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionAny>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool retainAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionAny>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    void clear() override { throw ::jxx::lang::UnsupportedOperationException(); }

private:
    ::jxx::Ptr<::jxx::util::Set<E>> delegate_;
};

template <typename K, typename V>
class ReadOnlyEntrySet final
    : public ::jxx::lang::ClassBase<
          ReadOnlyEntrySet<K, V>,
          ::jxx::lang::Object,
          ::jxx::util::Set<::jxx::util::MapEntry<K, V>>> {
public:
    using Entry = ::jxx::util::MapEntry<K, V>;
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<ReadOnlyEntrySet<K, V>, JxxSuper, ::jxx::util::Set<Entry>>;

    explicit ReadOnlyEntrySet(const ::jxx::Ptr<::jxx::util::Set<Entry>>& delegate)
        : Super(), delegate_(delegate)
    {
        if (delegate_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    ::jxx::lang::jint size() override { return delegate_->size(); }
    ::jxx::lang::jbool isEmpty() override { return delegate_->isEmpty(); }
    ::jxx::lang::jbool contains(const ::jxx::Ptr<::jxx::lang::Object>& value) override { return delegate_->contains(value); }
    ::jxx::Ptr<::jxx::util::Iterator<Entry>> iterator() override
    {
        return ::jxx::CAST<::jxx::util::Iterator<Entry>>(
            ::jxx::NEW<ReadOnlyEntryIterator<K, V>>(delegate_->iterator()));
    }
    ::jxx::lang::ObjectArray toArray() override { return delegate_->toArray(); }
    ::jxx::lang::jbool containsAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionAny>& values) override { return delegate_->containsAll(values); }
    ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override { return delegate_->equals(object); }
    ::jxx::lang::jint hashCode() const override { return delegate_->hashCode(); }
    ::jxx::Ptr<::jxx::util::Spliterator<Entry>> spliterator() override
    {
        return delegate_->spliterator();
    }
    ::jxx::lang::jbool add(const ::jxx::Ptr<Entry>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool remove(const ::jxx::Ptr<::jxx::lang::Object>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool addAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionExtends<Entry>>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool removeAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionAny>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    ::jxx::lang::jbool retainAll(const ::jxx::Ptr<::jxx::util::wildcard::CollectionAny>&) override { throw ::jxx::lang::UnsupportedOperationException(); }
    void clear() override { throw ::jxx::lang::UnsupportedOperationException(); }

private:
    ::jxx::Ptr<::jxx::util::Set<Entry>> delegate_;
};

} // namespace jxx::com::sun::net::httpserver::internal
