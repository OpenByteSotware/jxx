#pragma once
#ifndef __JXX_LINKEDLIST__
#define __JXX_LINKEDLIST__
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <ostream>
#include <istream>
#include <limits>
#include <type_traits>
#include <optional>
#include <vector>
#include <iterator>
#include "jxx.io.Serializable.h"

// ====================== Java-like exceptions ======================
struct index_out_of_bounds_error : std::out_of_range {
    explicit index_out_of_bounds_error(const char* msg) : std::out_of_range(msg) {}
};
struct no_such_element_error : std::out_of_range {
    explicit no_such_element_error(const char* msg) : std::out_of_range(msg) {}
};
struct illegal_state_error : std::logic_error {
    explicit illegal_state_error(const char* msg) : std::logic_error(msg) {}
};
struct concurrent_modification_error : std::runtime_error {
    explicit concurrent_modification_error(const char* msg) : std::runtime_error(msg) {}
};

namespace jxx::util {
// ====================== Interfaces ======================
template <typename T>
struct List {
    virtual ~List() = default;

    virtual std::size_t size() const noexcept = 0;
    virtual bool empty() const noexcept = 0;

    virtual void clear() = 0;

    virtual const T& get(std::size_t index) const = 0;
    virtual T& get(std::size_t index) = 0;

    // Java List#set returns previous element; we keep void (see setRetPrev below).
    virtual void set(std::size_t index, const T& value) = 0;
    virtual void set(std::size_t index, T&& value) = 0;

    virtual void add(const T& value) = 0;                 // add at end
    virtual void add(T&& value) = 0;                      // add at end
    virtual void add(std::size_t index, const T& value) = 0;
    virtual void add(std::size_t index, T&& value) = 0;

    // Java List.remove(int index) returns removed element
    virtual T removeAtRet(std::size_t index) = 0;
};

template <typename T>
struct Deque {
    virtual ~Deque() = default;

    // addFirst/Last (unbounded → always succeeds)
    virtual void addFirst(const T& e) = 0;
    virtual void addFirst(T&& e) = 0;
    virtual void addLast(const T& e) = 0;
    virtual void addLast(T&& e) = 0;

    // offerFirst/Last: boolean (always true here)
    virtual bool offerFirst(const T& e) = 0;
    virtual bool offerFirst(T&& e) = 0;
    virtual bool offerLast(const T& e) = 0;
    virtual bool offerLast(T&& e) = 0;

    // removeFirst/Last: throws if empty
    virtual T removeFirstRet() = 0;
    virtual T removeLastRet() = 0;

    // pollFirst/Last: empty optional if empty
    virtual std::optional<T> pollFirst() = 0;
    virtual std::optional<T> pollLast() = 0;

    // Accessors
    virtual const T& getFirst() const = 0;
    virtual const T& getLast() const = 0;

    // Stack ops
    virtual void push(const T& e) = 0;
    virtual void push(T&& e) = 0;
    virtual T popRet() = 0;
};

struct Cloneable {
    virtual ~Cloneable() = default;
    virtual std::unique_ptr<Cloneable> clone_base() const = 0;
};

// ====================== AbstractSequentialList ======================
template <typename T>
class AbstractSequentialList : public List<T> {
public:
    ~AbstractSequentialList() override = default;
    bool empty() const noexcept override { return this->size() == 0; }
    void add(const T& value) override { this->add(this->size(), value); }
    void add(T&& value) override { this->add(this->size(), std::move(value)); }
};

// ====================== LinkedList (Java 8 surface) ======================
template <typename T>
class LinkedList
    : public AbstractSequentialList<T>
    , public Deque<T>
    , public Cloneable
    , public jxx::io::Serializable
{
private:
    struct Node {
        T item;
        Node* prev{ nullptr };
        Node* next{ nullptr };
        template <typename U>
        explicit Node(U&& v) : item(std::forward<U>(v)) {}
    };

    Node* first_{ nullptr };
    Node* last_{ nullptr };
    std::size_t size_{ 0 };
    std::size_t mod_count_{ 0 };  // fail-fast like AbstractList.modCount

    static bool equalsElem(const T& a, const T& b) { return a == b; }

    Node* nodeAt(std::size_t index) const {
        if (index >= size_) throw index_out_of_bounds_error("index out of bounds");
        if (index < (size_ / 2)) {
            Node* x = first_;
            for (std::size_t i = 0; i < index; ++i) x = x->next;
            return x;
        }
        else {
            Node* x = last_;
            for (std::size_t i = size_ - 1; i > index; --i) x = x->prev;
            return x;
        }
    }

    void linkFirst(Node* n) {
        Node* f = first_;
        n->next = f; n->prev = nullptr;
        first_ = n;
        if (!f) last_ = n; else f->prev = n;
        ++size_; ++mod_count_;
    }
    void linkLast(Node* n) {
        Node* l = last_;
        n->prev = l; n->next = nullptr;
        last_ = n;
        if (!l) first_ = n; else l->next = n;
        ++size_; ++mod_count_;
    }
    void linkBefore(Node* succ, Node* n) {
        if (!succ) { linkLast(n); return; }
        Node* pred = succ->prev;
        n->next = succ; n->prev = pred;
        succ->prev = n;
        if (pred) pred->next = n; else first_ = n;
        ++size_; ++mod_count_;
    }
    T unlink(Node* x) {
        Node* next = x->next;
        Node* prev = x->prev;
        if (prev) prev->next = next; else first_ = next;
        if (next) next->prev = prev; else last_ = prev;
        T value = std::move(x->item);
        delete x;
        --size_; ++mod_count_;
        return value;
    }

protected:
    // ===== protected removeRange (like AbstractList.removeRange) =====
    void removeRange(std::size_t fromIndex, std::size_t toIndex) {
        if (fromIndex > toIndex) throw index_out_of_bounds_error("removeRange: from>to");
        if (toIndex > size_) throw index_out_of_bounds_error("removeRange: to>size");
        if (fromIndex == toIndex) return;
        // find first to remove and last to remove
        Node* start = nodeAt(fromIndex);
        Node* endSucc = (toIndex == size_) ? nullptr : nodeAt(toIndex); // successor after range
        // unlink block [start, endSucc)
        Node* p = start->prev;
        Node* cur = start;
        while (cur != endSucc) {
            Node* nx = cur->next;
            delete cur;
            --size_;
            cur = nx;
        }
        if (p) p->next = endSucc; else first_ = endSucc;
        if (endSucc) endSucc->prev = p; else last_ = p;
        ++mod_count_;
    }

public:
    // --------------- ctors/dtor/assign ---------------
    LinkedList() = default;

    template <typename Iterable>
    explicit LinkedList(const Iterable& c) {
        addAll(c);
    }

    LinkedList(std::initializer_list<T> init) {
        for (const auto& v : init) linkLast(new Node(v));
    }

    LinkedList(const LinkedList& rhs) {
        for (Node* x = rhs.first_; x; x = x->next) linkLast(new Node(x->item));
    }

    LinkedList(LinkedList&& rhs) noexcept
        : first_(rhs.first_), last_(rhs.last_), size_(rhs.size_), mod_count_(rhs.mod_count_) {
        rhs.first_ = rhs.last_ = nullptr; rhs.size_ = 0; rhs.mod_count_ = 0;
    }

    LinkedList& operator=(const LinkedList& rhs) {
        if (this != &rhs) {
            clear();
            for (Node* x = rhs.first_; x; x = x->next) linkLast(new Node(x->item));
        }
        return *this;
    }
    LinkedList& operator=(LinkedList&& rhs) noexcept {
        if (this != &rhs) {
            clear();
            first_ = rhs.first_; last_ = rhs.last_; size_ = rhs.size_; mod_count_ = rhs.mod_count_;
            rhs.first_ = rhs.last_ = nullptr; rhs.size_ = 0; rhs.mod_count_ = 0;
        }
        return *this;
    }

    ~LinkedList() override { clear(); }

    // =============== Iterators (fail-fast) ===============
    class ListIterator {
        LinkedList* list_{ nullptr };
        Node* next_{ nullptr };          // node to return on next()
        Node* lastReturned_{ nullptr };  // last returned by next()/previous()
        std::size_t nextIndex_{ 0 };
        std::size_t expected_mod_count_{ 0 };

        void checkForComodification() const {
            if (expected_mod_count_ != list_->mod_count_)
                throw concurrent_modification_error("Concurrent modification detected");
        }

    public:
        using index_type = std::ptrdiff_t; // allows -1 (Java style)

        ListIterator(LinkedList* list, std::size_t index)
            : list_(list), expected_mod_count_(list->mod_count_) {
            if (index > list->size_) throw index_out_of_bounds_error("listIterator index");
            next_ = (index == list->size_) ? nullptr : list->nodeAt(index);
            nextIndex_ = index;
        }

        bool hasNext() const { return nextIndex_ < list_->size_; }
        bool hasPrevious() const { return nextIndex_ > 0; }

        T& next() {
            checkForComodification();
            if (!hasNext()) throw no_such_element_error("next");
            lastReturned_ = next_;
            next_ = next_->next;
            ++nextIndex_;
            return lastReturned_->item;
        }

        T& previous() {
            checkForComodification();
            if (!hasPrevious()) throw no_such_element_error("previous");
            if (next_) lastReturned_ = next_->prev;
            else       lastReturned_ = list_->last_;
            next_ = lastReturned_;
            --nextIndex_;
            return lastReturned_->item;
        }

        index_type nextIndex() const { return static_cast<index_type>(nextIndex_); }
        index_type previousIndex() const { return hasPrevious() ? static_cast<index_type>(nextIndex_ - 1) : -1; }

        void remove() {
            checkForComodification();
            if (!lastReturned_) throw illegal_state_error("Iterator.remove illegal state");
            Node* toRemove = lastReturned_;
            if (toRemove == next_) {
                next_ = next_->next;
            }
            else {
                --nextIndex_;
            }
            (void)list_->unlink(toRemove);
            expected_mod_count_ = list_->mod_count_;
            lastReturned_ = nullptr;
        }

        void set(const T& e) {
            checkForComodification();
            if (!lastReturned_) throw illegal_state_error("Iterator.set illegal state");
            lastReturned_->item = e; // non-structural
        }
        void set(T&& e) {
            checkForComodification();
            if (!lastReturned_) throw illegal_state_error("Iterator.set illegal state");
            lastReturned_->item = std::move(e);
        }

        void add(const T& e) {
            checkForComodification();
            if (!next_) list_->linkLast(new Node(e));
            else        list_->linkBefore(next_, new Node(e));
            ++nextIndex_;
            expected_mod_count_ = list_->mod_count_;
            lastReturned_ = nullptr;
        }
        void add(T&& e) {
            checkForComodification();
            if (!next_) list_->linkLast(new Node(std::move(e)));
            else        list_->linkBefore(next_, new Node(std::move(e)));
            ++nextIndex_;
            expected_mod_count_ = list_->mod_count_;
            lastReturned_ = nullptr;
        }
    };

    class Iterator {
        ListIterator it_;
    public:
        explicit Iterator(LinkedList* list) : it_(list, 0) {}
        bool hasNext() const { return it_.hasNext(); }
        T& next() { return it_.next(); }
        void remove() { it_.remove(); }
        void set(const T& e) { it_.set(e); }
        void set(T&& e) { it_.set(std::move(e)); }
        void add(const T& e) { it_.add(e); }
        void add(T&& e) { it_.add(std::move(e)); }
    };

    class DescendingIterator {
        ListIterator it_;
    public:
        explicit DescendingIterator(LinkedList* list) : it_(list, list->size_) {}
        bool hasNext() const { return it_.hasPrevious(); }
        T& next() { return it_.previous(); }
        void remove() { it_.remove(); }
    };

    // =============== IList<T> ===============
    std::size_t size() const noexcept override { return size_; }
    bool empty() const noexcept override { return size_ == 0; }
    bool isEmpty() const noexcept { return empty(); } // Java name

    void clear() override {
        Node* x = first_;
        while (x) {
            Node* nx = x->next;
            delete x;
            x = nx;
        }
        first_ = last_ = nullptr;
        if (size_ != 0) { size_ = 0; ++mod_count_; }
    }

    const T& get(std::size_t index) const override { return nodeAt(index)->item; }
    T& get(std::size_t index)       override { return nodeAt(index)->item; }

    // Java List#set returns previous; add helper returning previous if needed
    void set(std::size_t index, const T& value) override { nodeAt(index)->item = value; }
    void set(std::size_t index, T&& value)      override { nodeAt(index)->item = std::move(value); }
    T setRetPrev(std::size_t index, const T& value) { Node* n = nodeAt(index); T prev = n->item; n->item = value; return prev; }
    T setRetPrev(std::size_t index, T&& value) { Node* n = nodeAt(index); T prev = std::move(n->item); n->item = std::move(value); return prev; }

    // add(e) returns void in our base; provide Java-port helper addBool() -> true
    void add(std::size_t index, const T& value) override {
        if (index > size_) throw index_out_of_bounds_error("add: index");
        if (index == size_) linkLast(new Node(value));
        else                linkBefore(nodeAt(index), new Node(value));
    }
    void add(std::size_t index, T&& value) override {
        if (index > size_) throw index_out_of_bounds_error("add: index");
        if (index == size_) linkLast(new Node(std::move(value)));
        else                linkBefore(nodeAt(index), new Node(std::move(value)));
    }
    bool addBool(const T& e) { this->add(e); return true; }
    bool addBool(T&& e) { this->add(std::move(e)); return true; }

    T removeAtRet(std::size_t index) override { return unlink(nodeAt(index)); }

    // =============== Java Collection & List extras ===============
    bool contains(const T& o) const { return indexOf(o) != static_cast<std::size_t>(-1); }

    std::size_t indexOf(const T& o) const {
        std::size_t i = 0;
        for (Node* x = first_; x; x = x->next, ++i) if (equalsElem(x->item, o)) return i;
        return static_cast<std::size_t>(-1);
    }
    std::size_t lastIndexOf(const T& o) const {
        if (size_ == 0) return static_cast<std::size_t>(-1);
        std::size_t i = size_ - 1;
        for (Node* x = last_; x; x = x->prev, --i) {
            if (equalsElem(x->item, o)) return i;
            if (i == 0) break;
        }
        return static_cast<std::size_t>(-1);
    }

    bool remove(const T& o) { // remove first occurrence
        for (Node* x = first_; x; x = x->next) {
            if (equalsElem(x->item, o)) { unlink(x); return true; }
        }
        return false;
    }

    // remove() (no-arg) → Deque.removeFirst() in Java
    T remove() { return removeFirstRet(); }

    // Collection-like bulk ops
    template <typename Iterable>
    bool containsAll(const Iterable& c) const {
        for (const auto& v : c) if (!contains(v)) return false;
        return true;
    }
    template <typename Iterable>
    bool addAll(const Iterable& c) {
        bool changed = false;
        for (const auto& v : c) { linkLast(new Node(v)); changed = true; }
        return changed;
    }
    template <typename Iterable>
    bool addAll(std::size_t index, const Iterable& c) {
        if (index > size_) throw index_out_of_bounds_error("addAll index");
        Node* succ = (index == size_) ? nullptr : nodeAt(index);
        bool changed = false;
        for (const auto& v : c) {
            if (!succ) linkLast(new Node(v));
            else { linkBefore(succ, new Node(v)); ++index; }
            changed = true;
        }
        return changed;
    }
    template <typename Iterable>
    bool removeAll(const Iterable& c) {
        bool changed = false;
        for (const auto& v : c) {
            while (remove(v)) changed = true;
        }
        return changed;
    }
    template <typename Iterable>
    bool retainAll(const Iterable& c) {
        // Naive O(n*m): acceptable unless c is huge; can optimize for hashable T if needed.
        bool changed = false;
        Node* x = first_;
        while (x) {
            Node* nx = x->next;
            bool keep = false;
            for (const auto& v : c) { if (equalsElem(x->item, v)) { keep = true; break; } }
            if (!keep) { (void)unlink(x); changed = true; }
            x = nx;
        }
        return changed;
    }

    // subList view (live, fail-fast)
    class SubList {
        LinkedList* parent_;
        std::size_t offset_;
        std::size_t size_; // number of elements in view
        std::size_t expected_mod_count_;

        void checkForComodification() const {
            if (expected_mod_count_ != parent_->mod_count_)
                throw concurrent_modification_error("SubList concurrent modification");
        }
        void rangeCheck(std::size_t idx) const {
            if (idx >= size_) throw index_out_of_bounds_error("SubList index");
        }

        std::size_t toParentIndex(std::size_t idx) const { return offset_ + idx; }

        void updateExpected() { expected_mod_count_ = parent_->mod_count_; }

    public:
        SubList(LinkedList* p, std::size_t from, std::size_t to)
            : parent_(p), offset_(from), size_(to - from), expected_mod_count_(p->mod_count_) {
            if (from > to || to > p->size_) throw index_out_of_bounds_error("subList range");
        }

        std::size_t size() const { return size_; }
        bool isEmpty() const { return size_ == 0; }

        T& get(std::size_t index) {
            checkForComodification(); rangeCheck(index);
            return parent_->get(toParentIndex(index));
        }
        const T& get(std::size_t index) const {
            checkForComodification(); rangeCheck(index);
            return parent_->get(toParentIndex(index));
        }

        T setRetPrev(std::size_t index, const T& e) {
            checkForComodification(); rangeCheck(index);
            return parent_->setRetPrev(toParentIndex(index), e);
        }
        T setRetPrev(std::size_t index, T&& e) {
            checkForComodification(); rangeCheck(index);
            return parent_->setRetPrev(toParentIndex(index), std::move(e));
        }
        void set(std::size_t index, const T& e) {
            checkForComodification(); rangeCheck(index);
            parent_->set(toParentIndex(index), e);
        }
        void set(std::size_t index, T&& e) {
            checkForComodification(); rangeCheck(index);
            parent_->set(toParentIndex(index), std::move(e));
        }

        void add(std::size_t index, const T& e) {
            checkForComodification();
            if (index > size_) throw index_out_of_bounds_error("SubList add index");
            parent_->add(toParentIndex(index), e);
            ++size_; updateExpected();
        }
        void add(std::size_t index, T&& e) {
            checkForComodification();
            if (index > size_) throw index_out_of_bounds_error("SubList add index");
            parent_->add(toParentIndex(index), std::move(e));
            ++size_; updateExpected();
        }
        void add(const T& e) { add(size_, e); }
        void add(T&& e) { add(size_, std::move(e)); }

        T removeAtRet(std::size_t index) {
            checkForComodification(); rangeCheck(index);
            T v = parent_->removeAtRet(toParentIndex(index));
            --size_; updateExpected();
            return v;
        }
        bool remove(const T& e) { // first occurrence within view
            checkForComodification();
            for (std::size_t i = 0; i < size_; ++i) {
                if (equalsElem(get(i), e)) { (void)removeAtRet(i); return true; }
            }
            return false;
        }

        void clear() {
            checkForComodification();
            parent_->removeRange(offset_, offset_ + size_);
            size_ = 0; updateExpected();
        }

        // Iterators limited to [offset_, offset_+size_)
        class Iterator {
            LinkedList* parent_;
            std::size_t offset_;
            std::size_t size_;
            std::size_t idx_{ 0 };
            typename LinkedList::ListIterator it_; // positioned at offset + idx
            std::size_t expected_mod_count_;
        public:
            Iterator(LinkedList* p, std::size_t off, std::size_t sz)
                : parent_(p), offset_(off), size_(sz), idx_(0), it_(p, off), expected_mod_count_(p->mod_count_) {}
            bool hasNext() const {
                if (expected_mod_count_ != parent_->mod_count_) throw concurrent_modification_error("SubList iter CME");
                return idx_ < size_;
            }
            T& next() {
                if (!hasNext()) throw no_such_element_error("SubList iter next");
                ++idx_;
                return it_.next();
            }
            void remove() {
                // Remove last returned by it_, ensure bounds
                it_.remove();
                --size_;
                expected_mod_count_ = parent_->mod_count_;
            }
        };

        Iterator iterator() { return Iterator(parent_, offset_, size_); }
    };

    SubList subList(std::size_t fromIndex, std::size_t toIndex) {
        return SubList(this, fromIndex, toIndex);
    }

    // =============== IDeque<T> ===============
    void addFirst(const T& e) override { linkFirst(new Node(e)); }
    void addFirst(T&& e) override { linkFirst(new Node(std::move(e))); }
    void addLast(const T& e) override { linkLast(new Node(e)); }
    void addLast(T&& e) override { linkLast(new Node(std::move(e))); }

    bool offerFirst(const T& e) override { addFirst(e); return true; }
    bool offerFirst(T&& e) override { addFirst(std::move(e)); return true; }
    bool offerLast(const T& e) override { addLast(e); return true; }
    bool offerLast(T&& e) override { addLast(std::move(e)); return true; }

    T removeFirstRet() override {
        if (!first_) throw no_such_element_error("removeFirst on empty");
        return unlink(first_);
    }
    T removeLastRet() override {
        if (!last_) throw no_such_element_error("removeLast on empty");
        return unlink(last_);
    }

    std::optional<T> pollFirst() override {
        if (!first_) return std::nullopt;
        return unlink(first_);
    }
    std::optional<T> pollLast() override {
        if (!last_) return std::nullopt;
        return unlink(last_);
    }

    const T& getFirst() const override {
        if (!first_) throw no_such_element_error("getFirst on empty");
        return first_->item;
    }
    const T& getLast() const override {
        if (!last_) throw no_such_element_error("getLast on empty");
        return last_->item;
    }

    // Queue-like aliases per Java
    bool offer(const T& e) { return offerLast(e); }
    bool offer(T&& e) { return offerLast(std::move(e)); }

    std::optional<T> poll() { return pollFirst(); }

    const T& element() const { return getFirst(); }

    std::optional<T> peek() const {
        if (!first_) return std::nullopt;
        return first_->item;
    }
    std::optional<T> peekFirst() const { return peek(); }
    std::optional<T> peekLast() const {
        if (!last_) return std::nullopt;
        return last_->item;
    }

    // Stack ops and aliases
    void push(const T& e) override { addFirst(e); }
    void push(T&& e) override { addFirst(std::move(e)); }
    T popRet() override { return removeFirstRet(); }
    T pop() { return popRet(); }   // Java name

    // Occurrence removal
    bool removeFirstOccurrence(const T& o) { return remove(o); }
    bool removeLastOccurrence(const T& o) {
        for (Node* x = last_; x; x = x->prev) {
            if (equalsElem(x->item, o)) { unlink(x); return true; }
        }
        return false;
    }

    // =============== Iteration factories (Java-style) ===============
    Iterator iterator() { return Iterator(this); }
    ListIterator listIterator(std::size_t index = 0) { return ListIterator(this, index); }
    DescendingIterator descendingIterator() { return DescendingIterator(this); }

    // =============== Spliterator (Java 8) ===============
    class Spliterator {
        LinkedList* list_;
        Node* cur_;
        std::size_t est_;                  // remaining size estimate
        std::size_t expected_mod_count_;   // fail-fast snapshot

        void checkCME() const {
            if (expected_mod_count_ != list_->mod_count_) throw concurrent_modification_error("Spliterator CME");
        }
    public:
        // Characteristics: ORDERED | SIZED | SUBSIZED
        static constexpr int ORDERED = 0x00000010;
        static constexpr int SIZED = 0x00000040;
        static constexpr int SUBSIZED = 0x00004000;

        Spliterator(LinkedList* list)
            : list_(list), cur_(list->first_), est_(list->size_), expected_mod_count_(list->mod_count_) {}

        std::size_t estimateSize() const { return est_; }
        int characteristics() const { return ORDERED | SIZED | SUBSIZED; }

        // trySplit: split roughly in half; O(n/2) to locate mid node
        std::optional<Spliterator> trySplit() {
            checkCME();
            if (est_ < 2 || !cur_) return std::nullopt;
            // find mid by advancing est_/2 steps
            std::size_t half = est_ / 2;
            Node* start = cur_;
            Node* mid = cur_;
            for (std::size_t i = 0; i < half; ++i) mid = mid->next;
            // left spliterator: [start, mid), size=half
            Spliterator left(*this);
            left.cur_ = start;
            left.est_ = half;
            // this becomes right: [mid, end), size=est_-half
            this->cur_ = mid;
            this->est_ -= half;
            return left;
        }

        template <typename Consumer>
        void forEachRemaining(Consumer&& action) {
            checkCME();
            while (cur_) {
                action(cur_->item);
                cur_ = cur_->next;
                --est_;
            }
        }

        bool tryAdvance(T& out) {
            checkCME();
            if (!cur_) return false;
            out = cur_->item;
            cur_ = cur_->next;
            --est_;
            return true;
        }
    };

    Spliterator spliterator() { return Spliterator(this); }

    // =============== Clone (shallow) ===============
    std::unique_ptr<Cloneable> clone_base() const override {
        return std::unique_ptr<Cloneable>(new LinkedList<T>(*this));
    }
    std::unique_ptr<LinkedList<T>> clone() const {
        return std::unique_ptr<LinkedList<T>>(new LinkedList<T>(*this));
    }
    // Convenience: value-returning clone (like a copy)
    LinkedList<T> cloneValue() const { return LinkedList<T>(*this); }

    // =============== Serializable (simple text format) ===============
    void serialize(std::ostream& os) const override {
        os << size_ << '\n';
        for (Node* x = first_; x; x = x->next) {
            os << x->item << '\n'; // requires operator<< for T
        }
    }
    static LinkedList<T> deserialize(std::istream& is) {
        LinkedList<T> list;
        std::size_t n{};
        if (!(is >> n)) throw std::runtime_error("deserialize: failed to read size");
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        for (std::size_t i = 0; i < n; ++i) {
            T v{};
            if (!(is >> v)) throw std::runtime_error("deserialize: failed to read element");
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            list.addLast(std::move(v));
        }
        return list;
    }

    // =============== Arrays / vectors (Java toArray equivalents) ===============
    std::vector<T> toArray() const {
        std::vector<T> v;
        v.reserve(size_);
        for (Node* x = first_; x; x = x->next) v.push_back(x->item);
        return v;
    }
    template <typename OutputIt>
    void toArray(OutputIt out) const {
        for (Node* x = first_; x; x = x->next) *out++ = x->item;
    }

    // =============== C++ iterators (optional convenience) ===============
    class std_iterator {
        Node* cur_{ nullptr };
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        std_iterator() = default;
        explicit std_iterator(Node* n) : cur_(n) {}
        reference operator*()  const { return cur_->item; }
        pointer   operator->() const { return &cur_->item; }
        std_iterator& operator++() { cur_ = cur_->next; return *this; }
        std_iterator operator++(int) { auto t = *this; ++(*this); return t; }
        std_iterator& operator--() { cur_ = cur_->prev; return *this; }
        std_iterator operator--(int) { auto t = *this; --(*this); return t; }
        friend bool operator==(const std_iterator& a, const std_iterator& b) { return a.cur_ == b.cur_; }
        friend bool operator!=(const std_iterator& a, const std_iterator& b) { return !(a == b); }
    };
    class const_std_iterator {
        const Node* cur_{ nullptr };
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        const_std_iterator() = default;
        explicit const_std_iterator(const Node* n) : cur_(n) {}
        reference operator*()  const { return cur_->item; }
        pointer   operator->() const { return &cur_->item; }
        const_std_iterator& operator++() { cur_ = cur_->next; return *this; }
        const_std_iterator operator++(int) { auto t = *this; ++(*this); return t; }
        const_std_iterator& operator--() { cur_ = cur_->prev; return *this; }
        const_std_iterator operator--(int) { auto t = *this; --(*this); return t; }
        friend bool operator==(const const_std_iterator& a, const const_std_iterator& b) { return a.cur_ == b.cur_; }
        friend bool operator!=(const const_std_iterator& a, const const_std_iterator& b) { return !(a == b); }
    };

    std_iterator begin() { return std_iterator(first_); }
    std_iterator end() { return std_iterator(nullptr); }
    const_std_iterator begin() const { return const_std_iterator(first_); }
    const_std_iterator end()   const { return const_std_iterator(nullptr); }
    const_std_iterator cbegin() const { return const_std_iterator(first_); }
    const_std_iterator cend()   const { return const_std_iterator(nullptr); }
};

}

#endif