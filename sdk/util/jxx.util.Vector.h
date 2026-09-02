#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.ListIterator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.RandomAccess.h"
#include "util/jxx.util.SubList.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"
#include "util/function/jxx.util.function.UnaryOperator.h"

namespace jxx::util {

/**
 * Java 8 java.util.Vector<E> for JXX/C++17.
 *
 * Java reference values are represented by jxx::Ptr<T>. Every public reference
 * parameter is passed as const jxx::Ptr<T>&. STL storage is private.
 */
template <typename E>
class Vector
    : public jxx::lang::ClassBase<
          Vector<E>,
          AbstractList<E>,
          RandomAccess,
          jxx::lang::Cloneable,
          jxx::io::SerializableI> {
private:
    static constexpr jxx::lang::jint DEFAULT_CAPACITY = 10;

    std::vector<jxx::Ptr<E>> elements_;
    jxx::lang::jint capacityIncrement_ = 0;
    jxx::lang::jint capacity_ = DEFAULT_CAPACITY;

    void rangeCheck_(jxx::lang::jint index) const {
        if (index < 0 || index >= static_cast<jxx::lang::jint>(elements_.size())) {
            throw jxx::lang::ArrayIndexOutOfBoundsException();
        }
    }

    void rangeCheckForAdd_(jxx::lang::jint index) const {
        if (index < 0 || index > static_cast<jxx::lang::jint>(elements_.size())) {
            throw jxx::lang::ArrayIndexOutOfBoundsException();
        }
    }

    void ensureCapacityUnlocked_(jxx::lang::jint minimumCapacity) {
        if (minimumCapacity <= capacity_) {
            return;
        }

        jxx::lang::jint candidate = capacity_;
        while (candidate < minimumCapacity) {
            const auto previous = candidate;

            if (capacityIncrement_ > 0) {
                if (candidate >
                    std::numeric_limits<jxx::lang::jint>::max() - capacityIncrement_) {
                    candidate = minimumCapacity;
                } else {
                    candidate += capacityIncrement_;
                }
            } else {
                if (candidate <= 0) {
                    candidate = 1;
                } else if (candidate >
                    std::numeric_limits<jxx::lang::jint>::max() / 2) {
                    candidate = minimumCapacity;
                } else {
                    candidate *= 2;
                }
            }

            if (candidate <= previous) {
                candidate = minimumCapacity;
                break;
            }
        }

        capacity_ = std::max(candidate, minimumCapacity);
        elements_.reserve(static_cast<std::size_t>(capacity_));
    }

    static jxx::lang::jbool elementEqualsObject_(
        const jxx::Ptr<E>& element,
        const jxx::Ptr<jxx::lang::Object>& object) {

        if (element == nullptr || object == nullptr) {
            return element == nullptr && object == nullptr;
        }

        const auto elementObject = jxx::CAST<jxx::lang::Object>(element);
        return elementObject != nullptr
            ? elementObject->equals(object)
            : element.get() == object.get();
    }

    static std::string elementText_(const jxx::Ptr<E>& element) {
        if (element == nullptr) {
            return "null";
        }

        const auto object = jxx::CAST<jxx::lang::Object>(element);
        if (object == nullptr) {
            return "<object>";
        }

        const auto text = object->toString();
        return text == nullptr ? std::string("null") : text->utf8();
    }

    class VectorListIterator final
        : public jxx::lang::ClassBase<
              VectorListIterator,
              jxx::lang::Object,
              ListIterator<E>> {
    private:
        jxx::Ptr<Vector<E>> owner_;
        jxx::lang::jint cursor_ = 0;
        jxx::lang::jint lastReturned_ = -1;
        jxx::lang::jint expectedModCount_ = 0;

        void checkForComodification_() const {
            if (owner_->modCount != expectedModCount_) {
                throw ConcurrentModificationException();
            }
        }

    public:
        VectorListIterator(
            const jxx::Ptr<Vector<E>>& owner,
            jxx::lang::jint index)
            : owner_(owner)
            , cursor_(index)
            , expectedModCount_(owner == nullptr ? 0 : owner->modCount) {

            if (owner_ == nullptr) {
                throw jxx::lang::NullPointerException();
            }
            owner_->rangeCheckForAdd_(index);
        }

        ~VectorListIterator() override = default;

        jxx::lang::jbool hasNext() override {
            return cursor_ < owner_->size();
        }

        jxx::Ptr<E> next() override {
            checkForComodification_();
            if (!hasNext()) {
                throw NoSuchElementException();
            }
            lastReturned_ = cursor_;
            return owner_->get(cursor_++);
        }

        jxx::lang::jbool hasPrevious() override {
            return cursor_ > 0;
        }

        jxx::Ptr<E> previous() override {
            checkForComodification_();
            if (!hasPrevious()) {
                throw NoSuchElementException();
            }
            --cursor_;
            lastReturned_ = cursor_;
            return owner_->get(cursor_);
        }

        jxx::lang::jint nextIndex() override {
            return cursor_;
        }

        jxx::lang::jint previousIndex() override {
            return cursor_ - 1;
        }

        void remove() override {
            checkForComodification_();
            if (lastReturned_ < 0) {
                throw jxx::lang::IllegalStateException();
            }

            owner_->remove(lastReturned_);
            if (lastReturned_ < cursor_) {
                --cursor_;
            }
            lastReturned_ = -1;
            expectedModCount_ = owner_->modCount;
        }

        void set(const jxx::Ptr<E>& element) override {
            checkForComodification_();
            if (lastReturned_ < 0) {
                throw jxx::lang::IllegalStateException();
            }
            owner_->set(lastReturned_, element);
        }

        void add(const jxx::Ptr<E>& element) override {
            checkForComodification_();
            owner_->add(cursor_, element);
            ++cursor_;
            lastReturned_ = -1;
            expectedModCount_ = owner_->modCount;
        }
    };

    class VectorEnumeration final
        : public jxx::lang::ClassBase<
              VectorEnumeration,
              jxx::lang::Object,
              Enumeration<E>> {
    private:
        jxx::Ptr<Vector<E>> owner_;
        jxx::lang::jint cursor_ = 0;

    public:
        explicit VectorEnumeration(const jxx::Ptr<Vector<E>>& owner)
            : owner_(owner) {
            if (owner_ == nullptr) {
                throw jxx::lang::NullPointerException();
            }
        }

        ~VectorEnumeration() override = default;

        jxx::lang::jbool hasMoreElements() override {
            return cursor_ < owner_->size();
        }

        jxx::Ptr<E> nextElement() override {
            if (!hasMoreElements()) {
                throw NoSuchElementException();
            }
            return owner_->get(cursor_++);
        }
    };

public:
    Vector() {
        elements_.reserve(static_cast<std::size_t>(capacity_));
    }

    explicit Vector(jxx::lang::jint initialCapacity)
        : capacity_(initialCapacity) {
        if (initialCapacity < 0) {
            throw jxx::lang::IllegalArgumentException();
        }
        elements_.reserve(static_cast<std::size_t>(capacity_));
    }

    Vector(
        jxx::lang::jint initialCapacity,
        jxx::lang::jint capacityIncrement)
        : capacityIncrement_(capacityIncrement)
        , capacity_(initialCapacity) {

        if (initialCapacity < 0) {
            throw jxx::lang::IllegalArgumentException();
        }
        elements_.reserve(static_cast<std::size_t>(capacity_));
    }

    explicit Vector(
        const jxx::Ptr<wildcard::CollectionExtends<E>>& collection)
        : Vector() {

        if (collection == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        addAll(collection);
        this->modCount = 0;
    }

    ~Vector() override = default;

    void trimToSize() {
        this->synchronized([&]() {
            std::vector<jxx::Ptr<E>> compact(elements_);
            elements_.swap(compact);
            capacity_ = static_cast<jxx::lang::jint>(elements_.size());
            ++this->modCount;
        });
    }

    void ensureCapacity(jxx::lang::jint minimumCapacity) {
        this->synchronized([&]() {
            if (minimumCapacity > 0) {
                ensureCapacityUnlocked_(minimumCapacity);
            }
        });
    }

    void setSize(jxx::lang::jint newSize) {
        this->synchronized([&]() {
            if (newSize < 0) {
                throw jxx::lang::ArrayIndexOutOfBoundsException();
            }
            if (newSize == static_cast<jxx::lang::jint>(elements_.size())) {
                return;
            }
            ensureCapacityUnlocked_(newSize);
            elements_.resize(static_cast<std::size_t>(newSize), nullptr);
            ++this->modCount;
        });
    }

    jxx::lang::jint capacity() const {
        return this->synchronized([&]() -> jxx::lang::jint {
            return capacity_;
        });
    }

    jxx::lang::jint size() override {
        return this->synchronized([&]() -> jxx::lang::jint {
            return static_cast<jxx::lang::jint>(elements_.size());
        });
    }

    jxx::lang::jbool isEmpty() override {
        return this->synchronized([&]() -> jxx::lang::jbool {
            return elements_.empty();
        });
    }

    jxx::Ptr<Enumeration<E>> elements() {
        auto self = jxx::CAST<Vector<E>>(this->thisPtr);
        if (self == nullptr) {
            throw jxx::lang::IllegalStateException();
        }
        return jxx::CAST<Enumeration<E>>(
            jxx::NEW<VectorEnumeration>(self));
    }

    jxx::lang::jbool contains(
        const jxx::Ptr<jxx::lang::Object>& object) override {
        return indexOf(object) >= 0;
    }

    jxx::lang::jint indexOf(
        const jxx::Ptr<jxx::lang::Object>& object) override {
        return indexOf(object, 0);
    }

    jxx::lang::jint indexOf(
        const jxx::Ptr<jxx::lang::Object>& object,
        jxx::lang::jint index) {

        return this->synchronized([&]() -> jxx::lang::jint {
            if (index < 0) {
                throw jxx::lang::ArrayIndexOutOfBoundsException();
            }
            for (auto current = index;
                 current < static_cast<jxx::lang::jint>(elements_.size());
                 ++current) {
                if (elementEqualsObject_(
                        elements_[static_cast<std::size_t>(current)], object)) {
                    return current;
                }
            }
            return -1;
        });
    }

    jxx::lang::jint lastIndexOf(
        const jxx::Ptr<jxx::lang::Object>& object) override {

        return lastIndexOf(
            object,
            static_cast<jxx::lang::jint>(elements_.size()) - 1);
    }

    jxx::lang::jint lastIndexOf(
        const jxx::Ptr<jxx::lang::Object>& object,
        jxx::lang::jint index) {

        return this->synchronized([&]() -> jxx::lang::jint {
            if (index >= static_cast<jxx::lang::jint>(elements_.size())) {
                throw jxx::lang::ArrayIndexOutOfBoundsException();
            }
            if (index < 0) {
                return -1;
            }
            for (auto current = index; current >= 0; --current) {
                if (elementEqualsObject_(
                        elements_[static_cast<std::size_t>(current)], object)) {
                    return current;
                }
            }
            return -1;
        });
    }

    jxx::Ptr<E> elementAt(jxx::lang::jint index) const {
        return get(index);
    }

    jxx::Ptr<E> firstElement() const {
        return this->synchronized([&]() -> jxx::Ptr<E> {
            if (elements_.empty()) {
                throw NoSuchElementException();
            }
            return elements_.front();
        });
    }

    jxx::Ptr<E> lastElement() const {
        return this->synchronized([&]() -> jxx::Ptr<E> {
            if (elements_.empty()) {
                throw NoSuchElementException();
            }
            return elements_.back();
        });
    }

    void setElementAt(
        const jxx::Ptr<E>& element,
        jxx::lang::jint index) {

        this->synchronized([&]() {
            rangeCheck_(index);
            elements_[static_cast<std::size_t>(index)] = element;
        });
    }

    void removeElementAt(jxx::lang::jint index) {
        (void)remove(index);
    }

    void insertElementAt(
        const jxx::Ptr<E>& element,
        jxx::lang::jint index) {
        add(index, element);
    }

    void addElement(const jxx::Ptr<E>& element) {
        (void)add(element);
    }

    jxx::lang::jbool removeElement(
        const jxx::Ptr<jxx::lang::Object>& object) {
        return remove(object);
    }

    void removeAllElements() {
        clear();
    }

    jxx::Ptr<E> get(jxx::lang::jint index) const override {
        return this->synchronized([&]() -> jxx::Ptr<E> {
            rangeCheck_(index);
            return elements_[static_cast<std::size_t>(index)];
        });
    }

    jxx::Ptr<E> set(
        jxx::lang::jint index,
        const jxx::Ptr<E>& element) override {

        return this->synchronized([&]() -> jxx::Ptr<E> {
            rangeCheck_(index);
            auto previous = elements_[static_cast<std::size_t>(index)];
            elements_[static_cast<std::size_t>(index)] = element;
            return previous;
        });
    }

    jxx::lang::jbool add(const jxx::Ptr<E>& element) override {
        this->synchronized([&]() {
            ensureCapacityUnlocked_(
                static_cast<jxx::lang::jint>(elements_.size()) + 1);
            elements_.push_back(element);
            ++this->modCount;
        });
        return true;
    }

    void add(
        jxx::lang::jint index,
        const jxx::Ptr<E>& element) override {

        this->synchronized([&]() {
            rangeCheckForAdd_(index);
            ensureCapacityUnlocked_(
                static_cast<jxx::lang::jint>(elements_.size()) + 1);
            elements_.insert(elements_.begin() + index, element);
            ++this->modCount;
        });
    }

    jxx::Ptr<E> remove(jxx::lang::jint index) override {
        return this->synchronized([&]() -> jxx::Ptr<E> {
            rangeCheck_(index);
            auto previous = elements_[static_cast<std::size_t>(index)];
            elements_.erase(elements_.begin() + index);
            ++this->modCount;
            return previous;
        });
    }

    jxx::lang::jbool remove(
        const jxx::Ptr<jxx::lang::Object>& object) override {

        return this->synchronized([&]() -> jxx::lang::jbool {
            for (auto iterator = elements_.begin(); iterator != elements_.end(); ++iterator) {
                if (elementEqualsObject_(*iterator, object)) {
                    elements_.erase(iterator);
                    ++this->modCount;
                    return true;
                }
            }
            return false;
        });
    }

    void clear() override {
        this->synchronized([&]() {
            if (!elements_.empty()) {
                elements_.clear();
                ++this->modCount;
            }
        });
    }

    jxx::lang::jbool addAll(
        const jxx::Ptr<wildcard::CollectionExtends<E>>& collection) override {
        return addAll(size(), collection);
    }

    jxx::lang::jbool addAll(
        jxx::lang::jint index,
        const jxx::Ptr<wildcard::CollectionExtends<E>>& collection) override {

        if (collection == nullptr) {
            throw jxx::lang::NullPointerException();
        }

        std::vector<jxx::Ptr<E>> incoming;
        auto source = collection->iteratorExtends();
        while (source->hasNext()) {
            incoming.push_back(source->next());
        }

        return this->synchronized([&]() -> jxx::lang::jbool {
            rangeCheckForAdd_(index);
            if (incoming.empty()) {
                return false;
            }
            ensureCapacityUnlocked_(
                static_cast<jxx::lang::jint>(elements_.size() + incoming.size()));
            elements_.insert(elements_.begin() + index, incoming.begin(), incoming.end());
            ++this->modCount;
            return true;
        });
    }

    jxx::Ptr<Iterator<E>> iterator() override {
        return jxx::CAST<Iterator<E>>(listIterator());
    }

    jxx::Ptr<ListIterator<E>> listIterator() override {
        return listIterator(0);
    }

    jxx::Ptr<ListIterator<E>> listIterator(jxx::lang::jint index) override {
        auto self = jxx::CAST<Vector<E>>(this->thisPtr);
        if (self == nullptr) {
            throw jxx::lang::IllegalStateException();
        }
        auto value = jxx::NEW<VectorListIterator>(self, index);
        return jxx::CAST<ListIterator<E>>(value);
    }

    jxx::Ptr<List<E>> subList(
        jxx::lang::jint fromIndex,
        jxx::lang::jint toIndex) override {

        if (fromIndex < 0 || toIndex < fromIndex || toIndex > size()) {
            throw jxx::lang::IndexOutOfBoundsException();
        }

        auto self = jxx::CAST<Vector<E>>(this->thisPtr);
        if (self == nullptr) {
            throw jxx::lang::IllegalStateException();
        }

        auto root = jxx::CAST<List<E>>(self);
        auto view = jxx::NEW<SubList<E>>(root, fromIndex, toIndex);
        return jxx::CAST<List<E>>(view);
    }

    jxx::lang::ObjectArray toArray() override {
        return this->synchronized([&]() -> jxx::lang::ObjectArray {
            auto result = jxx::NEW<jxx::lang::ObjectArrayType>(
                static_cast<std::uint32_t>(elements_.size()));

            for (std::size_t index = 0; index < elements_.size(); ++index) {
                (*result)[static_cast<jxx::lang::jint>(index)] =
                    jxx::CAST<jxx::lang::Object>(elements_[index]);
            }
            return result;
        });
    }

    void replaceAll(
        const jxx::Ptr<function::UnaryOperator<E>>& operation) {

        if (operation == nullptr) {
            throw jxx::lang::NullPointerException();
        }

        this->synchronized([&]() {
            const auto expected = this->modCount;
            for (auto& element : elements_) {
                element = operation->apply(element);
            }
            if (expected != this->modCount) {
                throw ConcurrentModificationException();
            }
            ++this->modCount;
        });
    }

    void sort(const jxx::Ptr<ComparatorSuper<E>>& comparator) {
        this->synchronized([&]() {
            const auto expected = this->modCount;
            std::stable_sort(
                elements_.begin(),
                elements_.end(),
                [&](const jxx::Ptr<E>& left, const jxx::Ptr<E>& right) {
                    if (comparator != nullptr) {
                        return comparator->compareSuper(left, right) < 0;
                    }
                    if (left == nullptr || right == nullptr) {
                        throw jxx::lang::NullPointerException();
                    }
                    const auto comparable =
                        jxx::CAST<jxx::lang::Comparable<E>>(left);
                    if (comparable == nullptr) {
                        throw jxx::lang::ClassCastException();
                    }
                    return comparable->compareTo(right) < 0;
                });

            if (expected != this->modCount) {
                throw ConcurrentModificationException();
            }
            ++this->modCount;
        });
    }

    jxx::lang::jbool equals(
        const jxx::Ptr<jxx::lang::Object>& object) const override {

        if (object == nullptr) {
            return false;
        }
        if (object.get() == this) {
            return true;
        }

        const auto other = jxx::CAST<List<E>>(object);
        if (other == nullptr) {
            return false;
        }

        return this->synchronized([&]() -> jxx::lang::jbool {
            if (static_cast<jxx::lang::jint>(elements_.size()) != other->size()) {
                return false;
            }

            auto otherIterator = other->iterator();
            for (const auto& element : elements_) {
                if (!otherIterator->hasNext()) {
                    return false;
                }
                const auto otherElement = otherIterator->next();
                const auto left = jxx::CAST<jxx::lang::Object>(element);
                const auto right = jxx::CAST<jxx::lang::Object>(otherElement);
                if (left == nullptr ? right != nullptr : !left->equals(right)) {
                    return false;
                }
            }
            return !otherIterator->hasNext();
        });
    }

    jxx::lang::jint hashCode() const override {
        return this->synchronized([&]() -> jxx::lang::jint {
            std::uint32_t hash = 1U;
            for (const auto& element : elements_) {
                const auto object = jxx::CAST<jxx::lang::Object>(element);
                const auto elementHash = object == nullptr ? 0 : object->hashCode();
                hash = hash * 31U + static_cast<std::uint32_t>(elementHash);
            }
            return static_cast<jxx::lang::jint>(static_cast<std::int32_t>(hash));
        });
    }

    jxx::Ptr<jxx::lang::String> toString() const override {
        return this->synchronized([&]() -> jxx::Ptr<jxx::lang::String> {
            std::ostringstream output;
            output << '[';
            for (std::size_t index = 0; index < elements_.size(); ++index) {
                if (index != 0) {
                    output << ", ";
                }
                output << elementText_(elements_[index]);
            }
            output << ']';
            return jxx::NEW<jxx::lang::String>(output.str());
        });
    }

protected:
    void removeRange(
        jxx::lang::jint fromIndex,
        jxx::lang::jint toIndex) {

        this->synchronized([&]() {
            if (fromIndex < 0 || toIndex < fromIndex ||
                toIndex > static_cast<jxx::lang::jint>(elements_.size())) {
                throw jxx::lang::IndexOutOfBoundsException();
            }
            if (fromIndex == toIndex) {
                return;
            }
            elements_.erase(elements_.begin() + fromIndex, elements_.begin() + toIndex);
            ++this->modCount;
        });
    }

    jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
        return this->synchronized([&]() -> jxx::Ptr<jxx::lang::Object> {
            auto copy = jxx::NEW<Vector<E>>(capacity_, capacityIncrement_);
            copy->elements_ = elements_;
            copy->modCount = 0;
            return jxx::CAST<jxx::lang::Object>(copy);
        });
    }

public:
    void writeObject(
        const jxx::Ptr<jxx::io::ObjectOutputStream>& output) override {
        if (output == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        throw jxx::lang::UnsupportedOperationException();
    }

    void readObject(
        const jxx::Ptr<jxx::io::ObjectInputStream>& input) override {
        if (input == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        throw jxx::lang::UnsupportedOperationException();
    }

    void readObjectNoData() override {
        this->synchronized([&]() {
            elements_.clear();
            capacityIncrement_ = 0;
            capacity_ = DEFAULT_CAPACITY;
            elements_.reserve(static_cast<std::size_t>(capacity_));
            ++this->modCount;
        });
    }
};

} // namespace jxx::util
