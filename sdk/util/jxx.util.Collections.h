#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <random>
#include <utility>
#include <vector>

#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Random.h"

namespace jxx::util {

    /**
     * Core Java 8 java.util.Collections algorithms for JXX/C++17.
     *
     * Collections is a non-instantiable, non-template utility class. Java generic
     * static methods are represented by C++ static function templates.
     *
     * Public APIs use JXX types only. STL containers are restricted to internal
     * temporary storage.
     */
    class Collections final : public jxx::lang::Object {
    private:
        Collections() = delete;

        template <typename T>
        static jxx::lang::jint compareNatural_(
            const jxx::Ptr<T>& left,
            const jxx::Ptr<T>& right) {

            if (left == nullptr || right == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            auto comparable = jxx::CAST<jxx::lang::Comparable<T>>(left);
            if (comparable == nullptr) {
                throw jxx::lang::ClassCastException();
            }

            return comparable->compareTo(right);
        }

        template <typename T>
        static jxx::lang::jint compare_(
            const jxx::Ptr<T>& left,
            const jxx::Ptr<T>& right,
            const jxx::Ptr<jxx::util::ComparatorSuper<T>>& comparator) {

            if (comparator != nullptr) {
                return comparator->compareSuper(left, right);
            }

            return compareNatural_(left, right);
        }

        static jxx::lang::jbool objectEquals_(
            const jxx::Ptr<jxx::lang::Object>& left,
            const jxx::Ptr<jxx::lang::Object>& right) {

            if (left == nullptr || right == nullptr) {
                return static_cast<jxx::lang::jbool>(left == right);
            }

            return left->equals(right);
        }

        template <typename T>
        static jxx::lang::jbool elementEquals_(
            const jxx::Ptr<T>& left,
            const jxx::Ptr<T>& right) {

            return objectEquals_(
                jxx::CAST<jxx::lang::Object>(left),
                jxx::CAST<jxx::lang::Object>(right));
        }

        template <typename T>
        static jxx::lang::jbool elementEqualsObject_(
            const jxx::Ptr<T>& left,
            const jxx::Ptr<jxx::lang::Object>& right) {

            return objectEquals_(jxx::CAST<jxx::lang::Object>(left), right);
        }

        template <typename T>
        static void reverseRange_(
            const jxx::Ptr<jxx::util::List<T>>& list,
            jxx::lang::jint fromIndex,
            jxx::lang::jint toIndex) {

            --toIndex;
            while (fromIndex < toIndex) {
                auto left = list->get(fromIndex);
                auto right = list->get(toIndex);
                list->set(fromIndex, right);
                list->set(toIndex, left);
                ++fromIndex;
                --toIndex;
            }
        }

    public:
        virtual ~Collections() = default;

        template <typename T>
        static void reverse(const jxx::Ptr<List<T>> list) {
            if (list == nullptr) {
                throw jxx::lang::NullPointerException();
            }
            reverseRange_(list, 0, list->size());
        }

        template <typename T>
        static void shuffle(const jxx::Ptr<List<T>> list) {
            if (list == nullptr) {
                throw jxx::lang::NullPointerException();
            }
            shuffle(list, jxx::NEW<Random>());
        }

        template <typename T>
        static void shuffle(const jxx::Ptr<List<T>> list, jxx::Ptr<jxx::util::Random> random) {
            if (list == nullptr || random == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            for (jxx::lang::jint i = list->size(); i > 1; --i) {
                const auto sourceIndex = i - 1;
                const auto targetIndex = random->nextInt(i);
                auto sourceValue = list->get(sourceIndex);
                auto targetValue = list->get(targetIndex);
                list->set(sourceIndex, targetValue);
                list->set(targetIndex, sourceValue);
            }
        }

        template <typename T>
        static void sort(const jxx::Ptr<List<T>> list) {
            sort<T>(list, nullptr);
        }

        template <typename T>
        static void sort(
            jxx::Ptr<List<T>> list,
            jxx::Ptr<ComparatorSuper<T>> comparator) {

            if (list == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            const auto count = list->size();
            std::vector<jxx::Ptr<T>> temporary;
            temporary.reserve(static_cast<std::size_t>(count));

            for (jxx::lang::jint i = 0; i < count; ++i) {
                temporary.push_back(list->get(i));
            }

            std::stable_sort(
                temporary.begin(),
                temporary.end(),
                [&](const jxx::Ptr<T>& left, const jxx::Ptr<T>& right) {
                    return compare_(left, right, comparator) < 0;
                });

            for (jxx::lang::jint i = 0; i < count; ++i) {
                list->set(i, temporary[static_cast<std::size_t>(i)]);
            }
        }

        template <typename T>
        static jxx::lang::jint binarySearch(
            jxx::Ptr<List<T>> list,
            jxx::Ptr<T> key) {

            return binarySearch<T>(list, key, nullptr);
        }

        template <typename T>
        static jxx::lang::jint binarySearch(
            jxx::Ptr<List<T>> list,
            jxx::Ptr<T> key,
            jxx::Ptr<ComparatorSuper<T>> comparator) {

            if (list == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            jxx::lang::jint low = 0;
            jxx::lang::jint high = list->size() - 1;

            while (low <= high) {
                const auto middle = low + ((high - low) >> 1);
                const auto comparison = compare_(list->get(middle), key, comparator);
                if (comparison < 0) {
                    low = middle + 1;
                }
                else if (comparison > 0) {
                    high = middle - 1;
                }
                else {
                    return middle;
                }
            }

            return -(low + 1);
        }

        template <typename T>
        static jxx::Ptr<T> min(const jxx::Ptr<Collection<T>> collection) {
            return min<T>(collection, nullptr);
        }

        template <typename T>
        static jxx::Ptr<T> min(
            jxx::Ptr<Collection<T>> collection,
            jxx::Ptr<ComparatorSuper<T>> comparator) {

            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            auto iterator = collection->iterator();
            if (!iterator->hasNext()) {
                throw jxx::util::NoSuchElementException();
            }

            auto candidate = iterator->next();
            while (iterator->hasNext()) {
                auto value = iterator->next();
                if (compare_(value, candidate, comparator) < 0) {
                    candidate = value;
                }
            }
            return candidate;
        }

        template <typename T>
        static jxx::Ptr<T> max(const jxx::Ptr<Collection<T>> collection) {
            return max<T>(collection, nullptr);
        }

        template <typename T>
        static jxx::Ptr<T> max(
            jxx::Ptr<Collection<T>> collection,
            jxx::Ptr<ComparatorSuper<T>> comparator) {

            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            auto iterator = collection->iterator();
            if (!iterator->hasNext()) {
                throw jxx::util::NoSuchElementException();
            }

            auto candidate = iterator->next();
            while (iterator->hasNext()) {
                auto value = iterator->next();
                if (compare_(value, candidate, comparator) > 0) {
                    candidate = value;
                }
            }
            return candidate;
        }

        static jxx::lang::jint frequency(
            jxx::Ptr<wildcard::CollectionAny> collection,
            jxx::Ptr<jxx::lang::Object> object) {

            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            jxx::lang::jint count = 0;
            auto iterator = collection->iteratorObject();
            while (iterator->hasNext()) {
                auto element = iterator->next();
                if (objectEquals_(object, element)) {
                    ++count;
                }
            }
            return count;
        }

        static jxx::lang::jbool disjoint(
            jxx::Ptr<wildcard::CollectionAny> first,
            jxx::Ptr<wildcard::CollectionAny> second) {

            if (first == nullptr || second == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            std::vector<jxx::Ptr<jxx::lang::Object>> secondElements;
            auto secondIterator = second->iteratorObject();
            while (secondIterator->hasNext()) {
                secondElements.push_back(secondIterator->next());
            }

            auto firstIterator = first->iteratorObject();
            while (firstIterator->hasNext()) {
                auto value = firstIterator->next();
                for (const auto& probe : secondElements) {
                    if (objectEquals_(value, probe)) {
                        return false;
                    }
                }
            }
            return true;
        }

        template <typename T>
        static void fill(const jxx::Ptr<List<T>> list, jxx::Ptr<T> value) {
            if (list == nullptr) {
                throw jxx::lang::NullPointerException();
            }
            for (jxx::lang::jint i = 0; i < list->size(); ++i) {
                list->set(i, value);
            }
        }

        template <typename T>
        static void copy(
            jxx::Ptr<List<T>> destination,
            jxx::Ptr<List<T>> source) {

            if (destination == nullptr || source == nullptr) {
                throw jxx::lang::NullPointerException();
            }
            if (source->size() > destination->size()) {
                throw jxx::lang::IndexOutOfBoundsException();
            }
            for (jxx::lang::jint i = 0; i < source->size(); ++i) {
                destination->set(i, source->get(i));
            }
        }

        template <typename T>
        static void swap(
            jxx::Ptr<List<T>> list,
            jxx::lang::jint first,
            jxx::lang::jint second) {

            if (list == nullptr) {
                throw jxx::lang::NullPointerException();
            }
            auto firstValue = list->get(first);
            auto secondValue = list->get(second);
            list->set(first, secondValue);
            list->set(second, firstValue);
        }

        template <typename T>
        static void rotate(const jxx::Ptr<List<T>> list, jxx::lang::jint distance) {
            if (list == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            const auto count = list->size();
            if (count == 0) {
                return;
            }

            auto normalized = distance % count;
            if (normalized < 0) {
                normalized += count;
            }
            if (normalized == 0) {
                return;
            }

            reverseRange_(list, 0, count);
            reverseRange_(list, 0, normalized);
            reverseRange_(list, normalized, count);
        }

        template <typename T>
        static jxx::lang::jbool replaceAll(
            jxx::Ptr<List<T>> list,
            jxx::Ptr<T> oldValue,
            jxx::Ptr<T> newValue) {

            if (list == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            jxx::lang::jbool modified = false;
            for (jxx::lang::jint i = 0; i < list->size(); ++i) {
                auto value = list->get(i);
                if (elementEquals_(value, oldValue)) {
                    list->set(i, newValue);
                    modified = true;
                }
            }
            return modified;
        }

        template <typename T>
        static jxx::lang::jint indexOfSubList(
            jxx::Ptr<List<T>> source,
            jxx::Ptr<List<T>> target) {

            if (source == nullptr || target == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            const auto sourceSize = source->size();
            const auto targetSize = target->size();
            if (targetSize == 0) return 0;
            if (targetSize > sourceSize) return -1;

            const auto lastCandidate = sourceSize - targetSize;
            for (jxx::lang::jint i = 0; i <= lastCandidate; ++i) {
                jxx::lang::jbool matches = true;
                for (jxx::lang::jint j = 0; j < targetSize; ++j) {
                    if (!elementEquals_(source->get(i + j), target->get(j))) {
                        matches = false;
                        break;
                    }
                }
                if (matches) return i;
            }
            return -1;
        }

        template <typename T>
        static jxx::lang::jint lastIndexOfSubList(
            jxx::Ptr<List<T>> source,
            jxx::Ptr<List<T>> target) {

            if (source == nullptr || target == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            const auto sourceSize = source->size();
            const auto targetSize = target->size();
            if (targetSize == 0) return sourceSize;
            if (targetSize > sourceSize) return -1;

            for (jxx::lang::jint i = sourceSize - targetSize; i >= 0; --i) {
                jxx::lang::jbool matches = true;
                for (jxx::lang::jint j = 0; j < targetSize; ++j) {
                    if (!elementEquals_(source->get(i + j), target->get(j))) {
                        matches = false;
                        break;
                    }
                }
                if (matches) return i;
            }
            return -1;
        }

        template <typename T>
        static jxx::lang::jbool addAll(const jxx::Ptr<Collection<T>>& collection,
            const jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<T>, 1U>>& elements) {

            if (collection == nullptr || elements == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            jxx::lang::jbool modified = false;
            for (jxx::lang::jint i = 0; i < elements->length; ++i) {
                if (collection->add((*elements)[i])) {
                    modified = true;
                }
            }
            return modified;
        }

    protected:
        jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
            throw jxx::lang::UnsupportedOperationException();
        }
    };

} // namespace jxx::util
