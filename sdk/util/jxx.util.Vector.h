#pragma once

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.RandomAccess.h"

namespace jxx {
namespace util {

// java.util.Vector compatible with the provided AbstractList<E> base shape.
// Template classes remain header-only per JXX constraints.
// Public reference-like values use jxx::Ptr<...>; STL is internal/private storage only.

template <typename E>
class Vector
    : public virtual AbstractList<E>
    , public virtual jxx::io::Serializable
    , public virtual jxx::lang::Cloneable
    , public virtual RandomAccess {
private:
    std::vector<jxx::Ptr<E>> elements_;
    jxx::lang::jint capacityIncrement_;
    jxx::lang::jint capacity_;

    static std::string ptrToString(const jxx::Ptr<E>& value) {
        if (value == nullptr) {
            return "null";
        }
        auto asObject = jxx::CAST<jxx::lang::Object>(value);
        if (asObject == nullptr) {
            return "<ptr>";
        }
        auto asString = asObject->toString();
        return asString == nullptr ? std::string("null") : asString->utf8();
    }

    static jxx::lang::jbool ptrEqualsObject(const jxx::Ptr<E>& a, const jxx::Ptr<jxx::lang::Object>& b) {
        if (a == nullptr && b == nullptr) {
            return static_cast<jxx::lang::jbool>(true);
        }
        if (a == nullptr || b == nullptr) {
            return static_cast<jxx::lang::jbool>(false);
        }
        auto ao = jxx::CAST<jxx::lang::Object>(a);
        if (ao == nullptr) {
            return static_cast<jxx::lang::jbool>(a.get() == b.get());
        }
        return ao->equals(b);
    }

    void ensureCapacityUnlocked(jxx::lang::jint minCapacity) {
        if (minCapacity <= capacity_) {
            return;
        }
        jxx::lang::jint newCapacity = capacity_ <= 0 ? 1 : capacity_;
        while (newCapacity < minCapacity) {
            if (capacityIncrement_ > 0) {
                newCapacity += capacityIncrement_;
            } else {
                newCapacity *= 2;
            }
            if (newCapacity <= 0) {
                newCapacity = minCapacity;
                break;
            }
        }
        capacity_ = std::max(newCapacity, minCapacity);
        if (elements_.capacity() < static_cast<std::size_t>(capacity_)) {
            elements_.reserve(static_cast<std::size_t>(capacity_));
        }
    }

    void rangeCheck(jxx::lang::jint index) const {
        if (index < 0 || index >= static_cast<jxx::lang::jint>(elements_.size())) {
            throw jxx::lang::ArrayIndexOutOfBoundsException();
        }
    }

    void rangeCheckForAddLocal(jxx::lang::jint index) const {
        if (index < 0 || index > static_cast<jxx::lang::jint>(elements_.size())) {
            throw jxx::lang::ArrayIndexOutOfBoundsException();
        }
    }

    class VectorEnumeration final : public virtual Enumeration<E> {
    private:
        jxx::Ptr<Vector<E>> owner_;
        mutable jxx::lang::jint cursor_;
    public:
        explicit VectorEnumeration(jxx::Ptr<Vector<E>> owner)
            : owner_(owner), cursor_(0) {
        }
        virtual ~VectorEnumeration() = default;

        virtual jxx::lang::jbool hasMoreElements() override {
            return owner_->synchronized([&]() -> jxx::lang::jbool {
                return static_cast<jxx::lang::jbool>(cursor_ < static_cast<jxx::lang::jint>(owner_->elements_.size()));
            });
        }

        virtual jxx::Ptr<E> nextElement() override {
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
        : elements_(), capacityIncrement_(0), capacity_(10) {
        elements_.reserve(static_cast<std::size_t>(capacity_));
    }

    explicit Vector(jxx::lang::jint initialCapacity)
        : elements_(), capacityIncrement_(0), capacity_(initialCapacity) {
        if (initialCapacity < 0) {
            throw jxx::lang::IllegalArgumentException();
        }
        elements_.reserve(static_cast<std::size_t>(capacity_));
    }

    Vector(jxx::lang::jint initialCapacity, jxx::lang::jint capacityIncrement)
        : elements_(), capacityIncrement_(capacityIncrement), capacity_(initialCapacity) {
        if (initialCapacity < 0) {
            throw jxx::lang::IllegalArgumentException();
        }
        elements_.reserve(static_cast<std::size_t>(capacity_));
    }

    explicit Vector(jxx::Ptr<wildcard::CollectionExtends<E>> c)
        : elements_(), capacityIncrement_(0), capacity_(10) {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        elements_.reserve(static_cast<std::size_t>(capacity_));
        addAll(c);
    }

    virtual ~Vector() = default;

    virtual void trimToSize() {
        this->synchronized([&]() {
            elements_.shrink_to_fit();
            capacity_ = static_cast<jxx::lang::jint>(elements_.capacity());
            ++this->modCount;
        });
    }

    virtual void ensureCapacity(jxx::lang::jint minCapacity) {
        this->synchronized([&]() {
            if (minCapacity > 0) {
                ensureCapacityUnlocked(minCapacity);
            }
        });
    }

    virtual void setSize(jxx::lang::jint newSize) {
        this->synchronized([&]() {
            if (newSize < 0) {
                throw jxx::lang::ArrayIndexOutOfBoundsException();
            }
            const auto oldSize = static_cast<jxx::lang::jint>(elements_.size());
            ensureCapacityUnlocked(newSize);
            if (newSize > oldSize) {
                elements_.resize(static_cast<std::size_t>(newSize), nullptr);
                ++this->modCount;
            } else if (newSize < oldSize) {
                elements_.resize(static_cast<std::size_t>(newSize));
                ++this->modCount;
            }
        });
    }

    virtual jxx::lang::jint capacity() {
        return this->synchronized([&]() -> jxx::lang::jint {
            return capacity_;
        });
    }

    virtual jxx::lang::jint size() override {
        return this->synchronized([&]() -> jxx::lang::jint {
            return static_cast<jxx::lang::jint>(elements_.size());
        });
    }

    virtual jxx::lang::jbool isEmpty() override {
        return this->synchronized([&]() -> jxx::lang::jbool {
            return static_cast<jxx::lang::jbool>(elements_.empty());
        });
    }

    virtual jxx::Ptr<Enumeration<E>> elements() {
        return this->synchronized([&]() -> jxx::Ptr<Enumeration<E>> {
            return std::make_shared<VectorEnumeration>(jxx::Ptr<Vector<E>>(this, [](Vector<E>*) {}));
        });
    }

    virtual jxx::lang::jint indexOf(jxx::Ptr<jxx::lang::Object> o) override {
        return indexOf(o, 0);
    }

    virtual jxx::lang::jint indexOf(jxx::Ptr<jxx::lang::Object> o, jxx::lang::jint index) {
        return this->synchronized([&]() -> jxx::lang::jint {
            if (index < 0) {
                index = 0;
            }
            for (std::size_t i = static_cast<std::size_t>(index); i < elements_.size(); ++i) {
                if (ptrEqualsObject(elements_[i], o)) {
                    return static_cast<jxx::lang::jint>(i);
                }
            }
            return static_cast<jxx::lang::jint>(-1);
        });
    }

    virtual jxx::lang::jint lastIndexOf(jxx::Ptr<jxx::lang::Object> o) override {
        return this->synchronized([&]() -> jxx::lang::jint {
            return lastIndexOf(o, static_cast<jxx::lang::jint>(elements_.empty() ? -1 : elements_.size() - 1));
        });
    }

    virtual jxx::lang::jint lastIndexOf(jxx::Ptr<jxx::lang::Object> o, jxx::lang::jint index) {
        return this->synchronized([&]() -> jxx::lang::jint {
            if (elements_.empty()) {
                return static_cast<jxx::lang::jint>(-1);
            }
            if (index >= static_cast<jxx::lang::jint>(elements_.size())) {
                throw jxx::lang::ArrayIndexOutOfBoundsException();
            }
            for (jxx::lang::jint i = index; i >= 0; --i) {
                if (ptrEqualsObject(elements_[static_cast<std::size_t>(i)], o)) {
                    return i;
                }
            }
            return static_cast<jxx::lang::jint>(-1);
        });
    }

    virtual jxx::Ptr<E> elementAt(jxx::lang::jint index) {
        return this->synchronized([&]() -> jxx::Ptr<E> {
            rangeCheck(index);
            return elements_[static_cast<std::size_t>(index)];
        });
    }

    virtual jxx::Ptr<E> firstElement() {
        return this->synchronized([&]() -> jxx::Ptr<E> {
            if (elements_.empty()) {
                throw jxx::util::NoSuchElementException();
            }
            return elements_.front();
        });
    }

    virtual jxx::Ptr<E> lastElement() {
        return this->synchronized([&]() -> jxx::Ptr<E> {
            if (elements_.empty()) {
                throw jxx::util::NoSuchElementException();
            }
            return elements_.back();
        });
    }

    virtual void setElementAt(jxx::Ptr<E> obj, jxx::lang::jint index) {
        this->synchronized([&]() {
            rangeCheck(index);
            elements_[static_cast<std::size_t>(index)] = obj;
        });
    }

    virtual void removeElementAt(jxx::lang::jint index) {
        this->synchronized([&]() {
            rangeCheck(index);
            elements_.erase(elements_.begin() + static_cast<long>(index));
            ++this->modCount;
        });
    }

    virtual void insertElementAt(jxx::Ptr<E> obj, jxx::lang::jint index) {
        this->synchronized([&]() {
            rangeCheckForAddLocal(index);
            ensureCapacityUnlocked(static_cast<jxx::lang::jint>(elements_.size()) + 1);
            elements_.insert(elements_.begin() + static_cast<long>(index), obj);
            ++this->modCount;
        });
    }

    virtual void addElement(jxx::Ptr<E> obj) {
        this->synchronized([&]() {
            ensureCapacityUnlocked(static_cast<jxx::lang::jint>(elements_.size()) + 1);
            elements_.push_back(obj);
            ++this->modCount;
        });
    }

    virtual jxx::lang::jbool removeElement(jxx::Ptr<jxx::lang::Object> obj) {
        return this->synchronized([&]() -> jxx::lang::jbool {
            const auto idx = indexOf(obj, 0);
            if (idx < 0) {
                return static_cast<jxx::lang::jbool>(false);
            }
            elements_.erase(elements_.begin() + static_cast<long>(idx));
            ++this->modCount;
            return static_cast<jxx::lang::jbool>(true);
        });
    }

    virtual void removeAllElements() {
        clear();
    }

    virtual jxx::Ptr<E> get(jxx::lang::jint index) override {
        return elementAt(index);
    }

    virtual jxx::Ptr<E> set(jxx::lang::jint index, jxx::Ptr<E> element) override {
        return this->synchronized([&]() -> jxx::Ptr<E> {
            rangeCheck(index);
            auto old = elements_[static_cast<std::size_t>(index)];
            elements_[static_cast<std::size_t>(index)] = element;
            return old;
        });
    }

    virtual jxx::lang::jbool add(jxx::Ptr<E> e) override {
        addElement(e);
        return static_cast<jxx::lang::jbool>(true);
    }

    virtual void add(jxx::lang::jint index, jxx::Ptr<E> element) override {
        insertElementAt(element, index);
    }

    virtual jxx::Ptr<E> remove(jxx::lang::jint index) override {
        return this->synchronized([&]() -> jxx::Ptr<E> {
            rangeCheck(index);
            auto old = elements_[static_cast<std::size_t>(index)];
            elements_.erase(elements_.begin() + static_cast<long>(index));
            ++this->modCount;
            return old;
        });
    }

    virtual void clear() override {
        this->synchronized([&]() {
            if (!elements_.empty()) {
                elements_.clear();
                ++this->modCount;
            }
        });
    }

    virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        return this->synchronized([&]() -> jxx::lang::jbool {
            auto it = c->iteratorExtends();
            jxx::lang::jbool modified = static_cast<jxx::lang::jbool>(false);
            while (it->hasNext()) {
                ensureCapacityUnlocked(static_cast<jxx::lang::jint>(elements_.size()) + 1);
                elements_.push_back(it->next());
                modified = static_cast<jxx::lang::jbool>(true);
            }
            if (modified) {
                ++this->modCount;
            }
            return modified;
        });
    }

    virtual jxx::lang::jbool addAll(jxx::lang::jint index, jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        return this->synchronized([&]() -> jxx::lang::jbool {
            rangeCheckForAddLocal(index);
            auto it = c->iteratorExtends();
            std::vector<jxx::Ptr<E>> incoming;
            while (it->hasNext()) {
                incoming.push_back(it->next());
            }
            if (incoming.empty()) {
                return static_cast<jxx::lang::jbool>(false);
            }
            ensureCapacityUnlocked(static_cast<jxx::lang::jint>(elements_.size() + incoming.size()));
            elements_.insert(elements_.begin() + static_cast<long>(index), incoming.begin(), incoming.end());
            ++this->modCount;
            return static_cast<jxx::lang::jbool>(true);
        });
    }

    virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        auto it = c->iteratorObject();
        while (it->hasNext()) {
            if (!this->contains(it->next())) {
                return static_cast<jxx::lang::jbool>(false);
            }
        }
        return static_cast<jxx::lang::jbool>(true);
    }

    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
        return removeElement(o);
    }

    virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        return this->synchronized([&]() -> jxx::lang::jbool {
            std::vector<jxx::Ptr<jxx::lang::Object>> removeSet;
            auto it = c->iteratorObject();
            while (it->hasNext()) {
                removeSet.push_back(it->next());
            }
            if (removeSet.empty()) {
                return static_cast<jxx::lang::jbool>(false);
            }
            std::vector<jxx::Ptr<E>> retained;
            retained.reserve(elements_.size());
            jxx::lang::jbool modified = static_cast<jxx::lang::jbool>(false);
            for (const auto& item : elements_) {
                jxx::lang::jbool found = static_cast<jxx::lang::jbool>(false);
                for (const auto& probe : removeSet) {
                    if (ptrEqualsObject(item, probe)) {
                        found = static_cast<jxx::lang::jbool>(true);
                        break;
                    }
                }
                if (found) {
                    modified = static_cast<jxx::lang::jbool>(true);
                } else {
                    retained.push_back(item);
                }
            }
            if (modified) {
                elements_.swap(retained);
                ++this->modCount;
            }
            return modified;
        });
    }

    virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        if (c == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        return this->synchronized([&]() -> jxx::lang::jbool {
            std::vector<jxx::Ptr<jxx::lang::Object>> keepSet;
            auto it = c->iteratorObject();
            while (it->hasNext()) {
                keepSet.push_back(it->next());
            }
            std::vector<jxx::Ptr<E>> retained;
            retained.reserve(elements_.size());
            jxx::lang::jbool modified = static_cast<jxx::lang::jbool>(false);
            for (const auto& item : elements_) {
                jxx::lang::jbool found = static_cast<jxx::lang::jbool>(false);
                for (const auto& probe : keepSet) {
                    if (ptrEqualsObject(item, probe)) {
                        found = static_cast<jxx::lang::jbool>(true);
                        break;
                    }
                }
                if (found) {
                    retained.push_back(item);
                } else {
                    modified = static_cast<jxx::lang::jbool>(true);
                }
            }
            if (modified) {
                elements_.swap(retained);
                ++this->modCount;
            }
            return modified;
        });
    }

    virtual jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
        return this->synchronized([&]() -> jxx::Ptr<jxx::lang::Object> {
            auto copy = std::make_shared<Vector<E>>(capacity_, capacityIncrement_);
            copy->elements_ = elements_;
            copy->capacity_ = capacity_;
            return jxx::CAST<jxx::lang::Object>(copy);
        });
    }

    virtual jxx::Ptr<jxx::lang::String> toString() const override {
        return this->synchronized([&]() -> jxx::Ptr<jxx::lang::String> {
            std::ostringstream oss;
            oss << "[";
            for (std::size_t i = 0; i < elements_.size(); ++i) {
                if (i != 0) {
                    oss << ", ";
                }
                oss << ptrToString(elements_[i]);
            }
            oss << "]";
            return std::make_shared<jxx::lang::String>(oss.str());
        });
    }
};

} // namespace util
} // namespace jxx
