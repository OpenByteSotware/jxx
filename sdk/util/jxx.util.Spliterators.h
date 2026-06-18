#pragma once
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.Spliterator.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "util/jxx.util.spliterators.ArrayRefSpliterator.h"
#include "util/jxx.util.spliterators.IteratorSpliterator.h"
#include "util/jxx.util.spliterators.EntryIteratorSpliterator.h"
#include "util/jxx.util.spliterators.KeyIteratorSpliterator.h"
#include "util/jxx.util.spliterators.ValueIteratorSpliterator.h"

namespace jxx {
namespace util {

class Spliterators : virtual public jxx::lang::Object {
private:
    Spliterators() = default;

public:
    template <typename E>
    static jxx::Ptr<jxx::util::Spliterator<E>> forArray(
        jxx::Ptr<JxxArray<jxx::Ptr<E>, 1U>> array,
        jxx::lang::jint characteristics = 0,
        jxx::Ptr<jxx::util::ComparatorSuper<E>> comparator = nullptr) {
        if (array == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        return jxx::Ptr<jxx::util::Spliterator<E>>(
            new jxx::util::spliterators::ArrayRefSpliterator<E>(array, 0, array->length(), characteristics, comparator));
    }

    template <typename E>
    static jxx::Ptr<jxx::util::Spliterator<E>> forIterator(
        jxx::Ptr<jxx::util::Iterator<E>> iterator,
        jxx::lang::jlong estimatedSize,
        jxx::lang::jint characteristics = 0,
        jxx::Ptr<jxx::util::ComparatorSuper<E>> comparator = nullptr) {
        return jxx::Ptr<jxx::util::Spliterator<E>>(
            new jxx::util::spliterators::IteratorSpliterator<E>(iterator, estimatedSize, characteristics, comparator));
    }

    template <typename E>
    static jxx::Ptr<jxx::util::Spliterator<E>> forCollection(
        jxx::Ptr<jxx::util::Iterator<E>> iterator,
        jxx::lang::jlong size,
        jxx::lang::jint extraCharacteristics = 0) {
        return forIterator<E>(
            iterator,
            size,
            jxx::util::Spliterator<E>::SIZED | jxx::util::Spliterator<E>::SUBSIZED | extraCharacteristics,
            nullptr);
    }

    template <typename E>
    static jxx::Ptr<jxx::util::Spliterator<E>> forSet(
        jxx::Ptr<jxx::util::Iterator<E>> iterator,
        jxx::lang::jlong size,
        jxx::lang::jint extraCharacteristics = 0) {
        return forIterator<E>(
            iterator,
            size,
            jxx::util::Spliterator<E>::DISTINCT |
            jxx::util::Spliterator<E>::SIZED |
            jxx::util::Spliterator<E>::SUBSIZED |
            extraCharacteristics,
            nullptr);
    }

    template <typename E>
    static jxx::Ptr<jxx::util::Spliterator<E>> forSortedSet(
        jxx::Ptr<jxx::util::Iterator<E>> iterator,
        jxx::lang::jlong size,
        jxx::Ptr<jxx::util::ComparatorSuper<E>> comparator,
        jxx::lang::jint extraCharacteristics = 0) {
        return forIterator<E>(
            iterator,
            size,
            jxx::util::Spliterator<E>::DISTINCT |
            jxx::util::Spliterator<E>::SORTED |
            jxx::util::Spliterator<E>::ORDERED |
            jxx::util::Spliterator<E>::SIZED |
            jxx::util::Spliterator<E>::SUBSIZED |
            extraCharacteristics,
            comparator);
    }

    template <typename K, typename V>
    static jxx::Ptr<jxx::util::Spliterator<jxx::util::MapEntry<K, V>>> forEntryIterator(
        jxx::Ptr<jxx::util::Iterator<jxx::util::MapEntry<K, V>>> iterator,
        jxx::lang::jlong size,
        jxx::lang::jint extraCharacteristics = 0) {
        return jxx::Ptr<jxx::util::Spliterator<jxx::util::MapEntry<K, V>>>(
            new jxx::util::spliterators::EntryIteratorSpliterator<K, V>(
                iterator,
                size,
                jxx::util::Spliterator<jxx::util::MapEntry<K, V>>::SIZED |
                jxx::util::Spliterator<jxx::util::MapEntry<K, V>>::SUBSIZED |
                extraCharacteristics));
    }

    template <typename K, typename V>
    static jxx::Ptr<jxx::util::Spliterator<K>> forKeyIterator(
        jxx::Ptr<jxx::util::Iterator<jxx::util::MapEntry<K, V>>> iterator,
        jxx::lang::jlong size,
        jxx::lang::jint extraCharacteristics = 0,
        jxx::Ptr<jxx::util::ComparatorSuper<K>> comparator = nullptr) {
        return jxx::Ptr<jxx::util::Spliterator<K>>(
            new jxx::util::spliterators::KeyIteratorSpliterator<K, V>(
                iterator,
                size,
                jxx::util::Spliterator<K>::DISTINCT |
                jxx::util::Spliterator<K>::SIZED |
                jxx::util::Spliterator<K>::SUBSIZED |
                extraCharacteristics,
                comparator));
    }

    template <typename K, typename V>
    static jxx::Ptr<jxx::util::Spliterator<V>> forValueIterator(
        jxx::Ptr<jxx::util::Iterator<jxx::util::MapEntry<K, V>>> iterator,
        jxx::lang::jlong size,
        jxx::lang::jint extraCharacteristics = 0) {
        return jxx::Ptr<jxx::util::Spliterator<V>>(
            new jxx::util::spliterators::ValueIteratorSpliterator<K, V>(
                iterator,
                size,
                jxx::util::Spliterator<V>::SIZED |
                jxx::util::Spliterator<V>::SUBSIZED |
                extraCharacteristics));
    }

    template <typename E>
    static jxx::Ptr<jxx::util::Spliterator<E>> spliteratorUnknownSize(
        jxx::Ptr<jxx::util::Iterator<E>> iterator,
        jxx::lang::jint characteristics = 0) {
        return forIterator<E>(iterator, -1LL, characteristics, nullptr);
    }
};

} // namespace util
} // namespace jxx
