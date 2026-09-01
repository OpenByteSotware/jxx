#pragma once

#include <algorithm>
#include <cstdint>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.RandomAccess.h"
#include "util/jxx.util.Spliterator.h"
#include "util/function/jxx.util.function.UnaryOperator.h"

namespace jxx::util {

    /**
     * Java 8 compatible java.util.Vector<E> for JXX/C++17.
     *
     * Public Java reference values use jxx::Ptr<T>. STL storage remains private.
     * All compound operations synchronize through Object::synchronized().
     */
    template <typename E>
    class Vector
        : public AbstractList<E>
        , public virtual jxx::io::SerializableI
        , public virtual jxx::lang::Cloneable
        , public virtual RandomAccess {
    private:
        static constexpr jxx::lang::jint DEFAULT_CAPACITY = 10;

        std::vector<jxx::Ptr<E>> elements_;
        jxx::lang::jint capacityIncrement_;
        jxx::lang::jint capacity_;

        static std::string ptrToString(const jxx::Ptr<E>& value) {
            if (value == nullptr) {
                return "null";
            }

            auto object = jxx::CAST<jxx::lang::Object>(value);
            if (object == nullptr) {
                return "<ptr>";
            }

            auto stringValue = object->toString();
            return stringValue == nullptr ? std::string("null") : stringValue->utf8();
        }

        static jxx::lang::jbool ptrEqualsObject(
            const jxx::Ptr<E>& left,
            const jxx::Ptr<jxx::lang::Object>& right) {

            if (left == nullptr || right == nullptr) {
                return static_cast<jxx::lang::jbool>(left == nullptr && right == nullptr);
            }

            auto leftObject = jxx::CAST<jxx::lang::Object>(left);
            if (leftObject == nullptr) {
                return static_cast<jxx::lang::jbool>(left.get() == right.get());
            }

            return leftObject->equals(right);
        }

        void ensureCapacityUnlocked(jxx::lang::jint minimumCapacity) {
            if (minimumCapacity <= capacity_) {
                return;
            }

            jxx::lang::jint newCapacity = capacity_;

            while (newCapacity < minimumCapacity) {
                const jxx::lang::jint previousCapacity = newCapacity;

                if (capacityIncrement_ > 0) {
                    if (newCapacity >
                        std::numeric_limits<jxx::lang::jint>::max() - capacityIncrement_) {
                        newCapacity = minimumCapacity;
                    }
                    else {
                        newCapacity += capacityIncrement_;
                    }
                }
                else {
                    if (newCapacity <= 0) {
                        newCapacity = 1;
                    }
                    else if (newCapacity >
                        std::numeric_limits<jxx::lang::jint>::max() / 2) {
                        newCapacity = minimumCapacity;
                    }
                    else {
                        newCapacity *= 2;
                    }
                }

                if (newCapacity <= previousCapacity) {
                    newCapacity = minimumCapacity;
                    break;
                }
            }

            capacity_ = std::max(newCapacity, minimumCapacity);
            elements_.reserve(static_cast<std::size_t>(capacity_));
        }

        void rangeCheck(jxx::lang::jint index) const {
            if (index < 0 ||
                index >= static_cast<jxx::lang::jint>(elements_.size())) {
                throw jxx::lang::ArrayIndexOutOfBoundsException();
            }
        }

        void rangeCheckForAddLocal(jxx::lang::jint index) const {
            if (index < 0 ||
                index > static_cast<jxx::lang::jint>(elements_.size())) {
                throw jxx::lang::ArrayIndexOutOfBoundsException();
            }
        }

        class VectorEnumeration final
            : public jxx::lang::Object
            , public virtual Enumeration<E> {
        private:
            Vector<E>* owner_;
            jxx::lang::jint cursor_;

        public:
            explicit VectorEnumeration(Vector<E>* owner)
                : owner_(owner), cursor_(0) {
                if (owner_ == nullptr) {
                    throw jxx::lang::NullPointerException();
                }
            }

            virtual ~VectorEnumeration() = default;

            jxx::lang::jbool hasMoreElements() override {
                return owner_->synchronized([&]() -> jxx::lang::jbool {
                    return static_cast<jxx::lang::jbool>(
                        cursor_ < static_cast<jxx::lang::jint>(owner_->elements_.size()));
                    });
            }

            jxx::Ptr<E> nextElement() override {
                return owner_->synchronized([&]() -> jxx::Ptr<E> {
                    if (cursor_ >= static_cast<jxx::lang::jint>(owner_->elements_.size())) {
                        throw jxx::util::NoSuchElementException();
                    }

                    return owner_->elements_[static_cast<std::size_t>(cursor_++)];
                    });
            }
        };

    public:
        Vector()
            : elements_(),
            capacityIncrement_(0),
            capacity_(DEFAULT_CAPACITY) {
            elements_.reserve(static_cast<std::size_t>(capacity_));
        }

        explicit Vector(jxx::lang::jint initialCapacity)
            : elements_(),
            capacityIncrement_(0),
            capacity_(initialCapacity) {
            if (initialCapacity < 0) {
                throw jxx::lang::IllegalArgumentException();
            }
            elements_.reserve(static_cast<std::size_t>(capacity_));
        }

        Vector(
            jxx::lang::jint initialCapacity,
            jxx::lang::jint capacityIncrement)
            : elements_(),
            capacityIncrement_(capacityIncrement),
            capacity_(initialCapacity) {
            if (initialCapacity < 0) {
                throw jxx::lang::IllegalArgumentException();
            }
            elements_.reserve(static_cast<std::size_t>(capacity_));
        }

        explicit Vector(const jxx::Ptr<wildcard::CollectionExtends<E>> collection)
            : elements_(),
            capacityIncrement_(0),
            capacity_(DEFAULT_CAPACITY) {
            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            const auto sourceSize = collection->size();
            if (sourceSize > capacity_) {
                capacity_ = sourceSize;
            }
            elements_.reserve(static_cast<std::size_t>(capacity_));
            addAll(collection);
        }

        virtual ~Vector() = default;

        void writeObject(const jxx::Ptr<jxx::io::ObjectOutputStream>& out) override {
            if (out == nullptr) {
                throw jxx::lang::NullPointerException();
            }
            throw jxx::lang::UnsupportedOperationException();
        }

        void readObject(const jxx::Ptr<jxx::io::ObjectInputStream>& in) override {
            if (in == nullptr) {
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

        void trimToSize() {
            this->synchronized([&]() {
                if (capacity_ != static_cast<jxx::lang::jint>(elements_.size())) {
                    std::vector<jxx::Ptr<E>> compact(elements_);
                    elements_.swap(compact);
                    capacity_ = static_cast<jxx::lang::jint>(elements_.size());
                    ++this->modCount;
                }
                });
        }

        void ensureCapacity(jxx::lang::jint minimumCapacity) {
            this->synchronized([&]() {
                if (minimumCapacity > 0) {
                    ensureCapacityUnlocked(minimumCapacity);
                }
                });
        }

        void setSize(jxx::lang::jint newSize) {
            this->synchronized([&]() {
                if (newSize < 0) {
                    throw jxx::lang::ArrayIndexOutOfBoundsException();
                }

                const auto oldSize = static_cast<jxx::lang::jint>(elements_.size());
                if (newSize == oldSize) {
                    return;
                }

                ensureCapacityUnlocked(newSize);
                elements_.resize(static_cast<std::size_t>(newSize), nullptr);
                ++this->modCount;
                });
        }

        jxx::lang::jint capacity() {
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
                return static_cast<jxx::lang::jbool>(elements_.empty());
                });
        }

        jxx::Ptr<Enumeration<E>> elements() {
            return jxx::NEW<VectorEnumeration>(this);
        }

        jxx::lang::jint indexOf(const jxx::Ptr<jxx::lang::Object> object) override {
            return indexOf(object, 0);
        }

        jxx::lang::jint indexOf(
            jxx::Ptr<jxx::lang::Object> object,
            jxx::lang::jint index) {
            return this->synchronized([&]() -> jxx::lang::jint {
                if (index < 0) {
                    throw jxx::lang::ArrayIndexOutOfBoundsException();
                }

                for (jxx::lang::jint i = index;
                    i < static_cast<jxx::lang::jint>(elements_.size());
                    ++i) {
                    if (ptrEqualsObject(elements_[static_cast<std::size_t>(i)], object)) {
                        return i;
                    }
                }
                return -1;
                });
        }

        jxx::lang::jint lastIndexOf(const jxx::Ptr<jxx::lang::Object> object) override {
            return this->synchronized([&]() -> jxx::lang::jint {
                for (jxx::lang::jint i =
                    static_cast<jxx::lang::jint>(elements_.size()) - 1;
                    i >= 0;
                    --i) {
                    if (ptrEqualsObject(elements_[static_cast<std::size_t>(i)], object)) {
                        return i;
                    }
                }
                return -1;
                });
        }

        jxx::lang::jint lastIndexOf(
            jxx::Ptr<jxx::lang::Object> object,
            jxx::lang::jint index) {
            return this->synchronized([&]() -> jxx::lang::jint {
                if (index < 0 ||
                    index >= static_cast<jxx::lang::jint>(elements_.size())) {
                    throw jxx::lang::ArrayIndexOutOfBoundsException();
                }

                for (jxx::lang::jint i = index; i >= 0; --i) {
                    if (ptrEqualsObject(elements_[static_cast<std::size_t>(i)], object)) {
                        return i;
                    }
                }
                return -1;
                });
        }

        jxx::Ptr<E> elementAt(jxx::lang::jint index) {
            return this->synchronized([&]() -> jxx::Ptr<E> {
                rangeCheck(index);
                return elements_[static_cast<std::size_t>(index)];
                });
        }

        jxx::Ptr<E> firstElement() {
            return this->synchronized([&]() -> jxx::Ptr<E> {
                if (elements_.empty()) {
                    throw jxx::util::NoSuchElementException();
                }
                return elements_.front();
                });
        }

        jxx::Ptr<E> lastElement() {
            return this->synchronized([&]() -> jxx::Ptr<E> {
                if (elements_.empty()) {
                    throw jxx::util::NoSuchElementException();
                }
                return elements_.back();
                });
        }

        void setElementAt(const jxx::Ptr<E> object, jxx::lang::jint index) {
            this->synchronized([&]() {
                rangeCheck(index);
                elements_[static_cast<std::size_t>(index)] = object;
                });
        }

        void removeElementAt(jxx::lang::jint index) {
            this->synchronized([&]() {
                rangeCheck(index);
                elements_.erase(elements_.begin() + index);
                ++this->modCount;
                });
        }

        void insertElementAt(const jxx::Ptr<E> object, jxx::lang::jint index) {
            this->synchronized([&]() {
                rangeCheckForAddLocal(index);
                ensureCapacityUnlocked(static_cast<jxx::lang::jint>(elements_.size()) + 1);
                elements_.insert(elements_.begin() + index, object);
                ++this->modCount;
                });
        }

        void addElement(const jxx::Ptr<E> object) {
            this->synchronized([&]() {
                ensureCapacityUnlocked(static_cast<jxx::lang::jint>(elements_.size()) + 1);
                elements_.push_back(object);
                ++this->modCount;
                });
        }

        jxx::lang::jbool removeElement(const jxx::Ptr<jxx::lang::Object> object) {
            return this->synchronized([&]() -> jxx::lang::jbool {
                for (auto iterator = elements_.begin(); iterator != elements_.end(); ++iterator) {
                    if (ptrEqualsObject(*iterator, object)) {
                        elements_.erase(iterator);
                        ++this->modCount;
                        return true;
                    }
                }
                return false;
                });
        }

        void removeAllElements() {
            clear();
        }

        jxx::Ptr<E> get(jxx::lang::jint index) override {
            return elementAt(index);
        }

        jxx::Ptr<E> set(jxx::lang::jint index, jxx::Ptr<E> element) override {
            return this->synchronized([&]() -> jxx::Ptr<E> {
                rangeCheck(index);
                auto old = elements_[static_cast<std::size_t>(index)];
                elements_[static_cast<std::size_t>(index)] = element;
                return old;
                });
        }

        jxx::lang::jbool add(const jxx::Ptr<E>& element) override {
            addElement(element);
            return true;
        }

        void add(jxx::lang::jint index, jxx::Ptr<E> element) override {
            insertElementAt(element, index);
        }

        jxx::Ptr<E> remove(jxx::lang::jint index) override {
            return this->synchronized([&]() -> jxx::Ptr<E> {
                rangeCheck(index);
                auto old = elements_[static_cast<std::size_t>(index)];
                elements_.erase(elements_.begin() + index);
                ++this->modCount;
                return old;
                });
        }

        jxx::lang::jbool remove(const jxx::Ptr<jxx::lang::Object>& object) override {
            return removeElement(object);
        }

        void clear() override {
            this->synchronized([&]() {
                if (!elements_.empty()) {
                    elements_.clear();
                    ++this->modCount;
                }
                });
        }

        jxx::lang::jbool addAll(const jxx::Ptr<wildcard::CollectionExtends<E>> collection) override {
            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            std::vector<jxx::Ptr<E>> incoming;
            auto iterator = collection->iteratorExtends();
            while (iterator->hasNext()) {
                incoming.push_back(iterator->next());
            }

            if (incoming.empty()) {
                return false;
            }

            return this->synchronized([&]() -> jxx::lang::jbool {
                ensureCapacityUnlocked(
                    static_cast<jxx::lang::jint>(elements_.size() + incoming.size()));
                elements_.insert(elements_.end(), incoming.begin(), incoming.end());
                ++this->modCount;
                return true;
                });
        }

        jxx::lang::jbool addAll(
            jxx::lang::jint index,
            jxx::Ptr<wildcard::CollectionExtends<E>> collection) override {
            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            std::vector<jxx::Ptr<E>> incoming;
            auto iterator = collection->iteratorExtends();
            while (iterator->hasNext()) {
                incoming.push_back(iterator->next());
            }

            return this->synchronized([&]() -> jxx::lang::jbool {
                rangeCheckForAddLocal(index);
                if (incoming.empty()) {
                    return false;
                }

                ensureCapacityUnlocked(
                    static_cast<jxx::lang::jint>(elements_.size() + incoming.size()));
                elements_.insert(elements_.begin() + index, incoming.begin(), incoming.end());
                ++this->modCount;
                return true;
                });
        }

        jxx::lang::jbool containsAll(const jxx::Ptr<wildcard::CollectionAny> collection) override {
            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            auto iterator = collection->iteratorObject();
            while (iterator->hasNext()) {
                if (!this->contains(iterator->next())) {
                    return false;
                }
            }
            return true;
        }

        jxx::lang::jbool removeAll(const jxx::Ptr<wildcard::CollectionAny> collection) override {
            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            std::vector<jxx::Ptr<jxx::lang::Object>> probes;
            auto iterator = collection->iteratorObject();
            while (iterator->hasNext()) {
                probes.push_back(iterator->next());
            }

            return this->synchronized([&]() -> jxx::lang::jbool {
                const auto oldSize = elements_.size();
                elements_.erase(
                    std::remove_if(
                        elements_.begin(),
                        elements_.end(),
                        [&](const jxx::Ptr<E>& element) {
                            for (const auto& probe : probes) {
                                if (ptrEqualsObject(element, probe)) {
                                    return true;
                                }
                            }
                            return false;
                        }),
                    elements_.end());

                if (elements_.size() != oldSize) {
                    ++this->modCount;
                    return true;
                }
                return false;
                });
        }

        jxx::lang::jbool retainAll(const jxx::Ptr<wildcard::CollectionAny> collection) override {
            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            std::vector<jxx::Ptr<jxx::lang::Object>> probes;
            auto iterator = collection->iteratorObject();
            while (iterator->hasNext()) {
                probes.push_back(iterator->next());
            }

            return this->synchronized([&]() -> jxx::lang::jbool {
                const auto oldSize = elements_.size();
                elements_.erase(
                    std::remove_if(
                        elements_.begin(),
                        elements_.end(),
                        [&](const jxx::Ptr<E>& element) {
                            for (const auto& probe : probes) {
                                if (ptrEqualsObject(element, probe)) {
                                    return false;
                                }
                            }
                            return true;
                        }),
                    elements_.end());

                if (elements_.size() != oldSize) {
                    ++this->modCount;
                    return true;
                }
                return false;
                });
        }

        jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
            return this->synchronized([&]() -> jxx::Ptr<jxx::lang::Object> {
                auto copy = jxx::NEW<Vector<E>>(capacity_, capacityIncrement_);
                copy->elements_ = elements_;
                copy->capacity_ = capacity_;
                return jxx::CAST<jxx::lang::Object>(copy);
                });
        }

        jxx::Ptr<jxx::lang::String> toString() const override {
            return this->synchronized([&]() -> jxx::Ptr<jxx::lang::String> {
                std::ostringstream output;
                output << "[";
                for (std::size_t i = 0; i < elements_.size(); ++i) {
                    if (i != 0) {
                        output << ", ";
                    }
                    output << ptrToString(elements_[i]);
                }
                output << "]";
                return jxx::NEW<jxx::lang::String>(output.str());
                });
        }

        jxx::Ptr<List<E>> subList(
            jxx::lang::jint fromIndex,
            jxx::lang::jint toIndex) override {
            return AbstractList<E>::subList(fromIndex, toIndex);
        }

        void replaceAll(const jxx::Ptr<function::UnaryOperator<E>> operation) override {
            if (operation == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            this->synchronized([&]() {
                for (auto& element : elements_) {
                    element = operation->apply(element);
                }
                });
        }

        void sort(const jxx::Ptr<ComparatorSuper<E>> comparator) override {
            this->synchronized([&]() {
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

                        auto comparable = jxx::CAST<jxx::lang::Comparable<E>>(left);
                        if (comparable == nullptr) {
                            throw jxx::lang::ClassCastException();
                        }

                        return comparable->compareTo(right) < 0;
                    });
                });
        }

        jxx::Ptr<Spliterator<E>>
            spliterator() override {

            throw jxx::lang::
                UnsupportedOperationException();
        }


        jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& object) override {
            if (object == nullptr) {
                return false;
            }
            if (object.get() == this) {
                return true;
            }

            auto other = jxx::CAST<List<E>>(object);
            if (other == nullptr) {
                return false;
            }

            return this->synchronized([&]() -> jxx::lang::jbool {
                if (static_cast<jxx::lang::jint>(elements_.size()) != other->size()) {
                    return false;
                }

                auto iterator = other->iterator();
                for (const auto& element : elements_) {
                    if (!iterator->hasNext()) {
                        return false;
                    }

                    auto otherElement = iterator->next();
                    if (element == nullptr || otherElement == nullptr) {
                        if (element != otherElement) {
                            return false;
                        }
                        continue;
                    }

                    auto leftObject = jxx::CAST<jxx::lang::Object>(element);
                    auto rightObject = jxx::CAST<jxx::lang::Object>(otherElement);

                    if (leftObject == nullptr || rightObject == nullptr) {
                        if (element.get() != otherElement.get()) {
                            return false;
                        }
                    }
                    else if (!leftObject->equals(rightObject)) {
                        return false;
                    }
                }

                return static_cast<jxx::lang::jbool>(!iterator->hasNext());
                });
        }

        jxx::lang::jint hashCode() override {
            return this->synchronized([&]() -> jxx::lang::jint {
                std::uint32_t hash = 1U;

                for (const auto& element : elements_) {
                    jxx::lang::jint elementHash = 0;
                    if (element != nullptr) {
                        auto object = jxx::CAST<jxx::lang::Object>(element);
                        if (object != nullptr) {
                            elementHash = object->hashCode();
                        }
                    }

                    hash = hash * 31U + static_cast<std::uint32_t>(elementHash);
                }

                return static_cast<jxx::lang::jint>(
                    static_cast<std::int32_t>(hash));
                });
        }
    };

} // namespace jxx::util
