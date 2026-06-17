#pragma once

#include <memory>
#include <set>
#include <shared_mutex>
#include <stdexcept>
#include <vector>

#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Comparable.h"
#include "io/jxx.io.Serializable.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.Comparator.h"
#include "util/jxx.util.NavigableSet.h"
#include "lang/jxx.lang.String.h"

namespace jxx::util
{
    template<typename E>
    class TreeSet final
        : public AbstractSet<E>
        , public NavigableSet<E>
        , public jxx::lang::Cloneable
        , public jxx::io::Serializable
    {
    private:
        struct ElementOrder
        {
            explicit ElementOrder(const TreeSet<E>* owner = nullptr)
                : owner_(owner)
            {
            }

            bool operator()(const jxx::Ptr<E>& a,
                            const jxx::Ptr<E>& b) const
            {
                return owner_->compare_(a, b) < 0;
            }

        private:
            const TreeSet<E>* owner_ = nullptr;
        };

        using NativeSet = std::set<jxx::Ptr<E>, ElementOrder>;

        class SnapshotIterator final : public jxx::lang::Object, public Iterator<E>
        {
        public:
            explicit SnapshotIterator(std::vector<jxx::Ptr<E>> items)
                : items_(std::move(items))
            {
            }

            ~SnapshotIterator() override = default;

        public:
            jxx::lang::jbool hasNext() const override
            {
                return index_ < items_.size();
            }

            jxx::Ptr<E> next() override
            {
                if (index_ >= items_.size())
                    return nullptr;
                return items_[index_++];
            }

            void remove() override
            {
                throw std::runtime_error("snapshot iterator remove unsupported");
            }

        private:
            std::vector<jxx::Ptr<E>> items_;
            std::size_t index_ = 0;
        };

    public:
        TreeSet()
            : comparator_(nullptr),
              set_(ElementOrder(this))
        {
        }

        explicit TreeSet(jxx::Ptr<Comparator<E>> comparator)
            : comparator_(std::move(comparator)),
              set_(ElementOrder(this))
        {
        }

        explicit TreeSet(jxx::Ptr<Collection<E>> c)
            : TreeSet()
        {
            if (c)
                this->addAll(c);
        }

        explicit TreeSet(jxx::Ptr<SortedSet<E>> s)
            : TreeSet(s ? s->comparator() : nullptr)
        {
            if (!s)
                return;
            for (jxx::lang::jint i = 0; i < s->size(); ++i)
                addElement(s->get(i));
        }

        ~TreeSet() override = default;

    public:
        jxx::lang::jint size() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return static_cast<jxx::lang::jint>(set_.size());
        }

        jxx::lang::jbool isEmpty() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return set_.empty();
        }

        jxx::lang::jbool contains(jxx::Ptr<E> e) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return set_.find(e) != set_.end();
        }

        jxx::Ptr<Iterator<E>> iterator() override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            std::vector<jxx::Ptr<E>> items;
            items.reserve(set_.size());
            for (const auto& x : set_)
                items.push_back(x);
            return std::make_shared<SnapshotIterator>(std::move(items));
        }

        jxx::Ptr<Iterator<E>> descendingIterator() override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            std::vector<jxx::Ptr<E>> items;
            items.reserve(set_.size());
            for (auto it = set_.rbegin(); it != set_.rend(); ++it)
                items.push_back(*it);
            return std::make_shared<SnapshotIterator>(std::move(items));
        }

        jxx::Ptr<E> get(jxx::lang::jint index) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            if (index < 0 || static_cast<std::size_t>(index) >= set_.size())
                return nullptr;
            auto it = set_.begin();
            std::advance(it, index);
            return *it;
        }

        void add(const jxx::Ptr<E>& value) override
        {
            addElement(value);
        }

        jxx::lang::jbool addAll(jxx::Ptr<Collection<E>> c) override
        {
            if (!c)
                return false;
            jxx::lang::jbool modified = false;
            for (jxx::lang::jint i = 0; i < c->size(); ++i)
                modified = addElement(c->get(i)) || modified;
            return modified;
        }

        jxx::lang::jbool addElement(jxx::Ptr<E> e) override
        {
            std::unique_lock<std::shared_mutex> lock(m_);
            return set_.insert(std::move(e)).second;
        }

        jxx::lang::jbool removeElement(jxx::Ptr<E> e) override
        {
            std::unique_lock<std::shared_mutex> lock(m_);
            auto it = set_.find(e);
            if (it == set_.end())
                return false;
            set_.erase(it);
            return true;
        }

        void clear() override
        {
            std::unique_lock<std::shared_mutex> lock(m_);
            set_.clear();
        }

        jxx::Ptr<Comparator<E>> comparator() const override
        {
            return comparator_;
        }

        jxx::Ptr<E> first() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return set_.empty() ? nullptr : *set_.begin();
        }

        jxx::Ptr<E> last() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return set_.empty() ? nullptr : *set_.rbegin();
        }

        jxx::Ptr<E> lower(jxx::Ptr<E> e) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            auto it = set_.lower_bound(e);
            if (it == set_.begin()) return nullptr;
            --it;
            return *it;
        }

        jxx::Ptr<E> floor(jxx::Ptr<E> e) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            auto it = set_.upper_bound(e);
            if (it == set_.begin()) return nullptr;
            --it;
            return *it;
        }

        jxx::Ptr<E> ceiling(jxx::Ptr<E> e) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            auto it = set_.lower_bound(e);
            return it == set_.end() ? nullptr : *it;
        }

        jxx::Ptr<E> higher(jxx::Ptr<E> e) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            auto it = set_.upper_bound(e);
            return it == set_.end() ? nullptr : *it;
        }

        jxx::Ptr<E> pollFirst() override
        {
            std::unique_lock<std::shared_mutex> lock(m_);
            if (set_.empty()) return nullptr;
            auto it = set_.begin();
            auto out = *it;
            set_.erase(it);
            return out;
        }

        jxx::Ptr<E> pollLast() override
        {
            std::unique_lock<std::shared_mutex> lock(m_);
            if (set_.empty()) return nullptr;
            auto it = std::prev(set_.end());
            auto out = *it;
            set_.erase(it);
            return out;
        }

        jxx::Ptr<SortedSet<E>> headSet(jxx::Ptr<E> toElement) const override
        {
            return std::static_pointer_cast<SortedSet<E>>(headSet(toElement, false));
        }

        jxx::Ptr<SortedSet<E>> subSet(jxx::Ptr<E> fromElement,
                                      jxx::Ptr<E> toElement) const override
        {
            return std::static_pointer_cast<SortedSet<E>>(subSet(fromElement, true, toElement, false));
        }

        jxx::Ptr<SortedSet<E>> tailSet(jxx::Ptr<E> fromElement) const override
        {
            return std::static_pointer_cast<SortedSet<E>>(tailSet(fromElement, true));
        }

        jxx::Ptr<NavigableSet<E>> headSet(jxx::Ptr<E> toElement,
                                          jxx::lang::jbool inclusive) const override
        {
            auto out = std::make_shared<TreeSet<E>>(comparator_);
            std::shared_lock<std::shared_mutex> lock(m_);
            for (const auto& x : set_)
            {
                const auto cmp = compare_(x, toElement);
                if (cmp < 0 || (inclusive && cmp == 0)) out->addElement(x);
                else break;
            }
            return out;
        }

        jxx::Ptr<NavigableSet<E>> tailSet(jxx::Ptr<E> fromElement,
                                          jxx::lang::jbool inclusive) const override
        {
            auto out = std::make_shared<TreeSet<E>>(comparator_);
            std::shared_lock<std::shared_mutex> lock(m_);
            for (const auto& x : set_)
            {
                const auto cmp = compare_(x, fromElement);
                if (cmp > 0 || (inclusive && cmp == 0)) out->addElement(x);
            }
            return out;
        }

        jxx::Ptr<NavigableSet<E>> subSet(jxx::Ptr<E> fromElement,
                                         jxx::lang::jbool fromInclusive,
                                         jxx::Ptr<E> toElement,
                                         jxx::lang::jbool toInclusive) const override
        {
            auto out = std::make_shared<TreeSet<E>>(comparator_);
            std::shared_lock<std::shared_mutex> lock(m_);
            for (const auto& x : set_)
            {
                const auto c1 = compare_(x, fromElement);
                const auto c2 = compare_(x, toElement);
                const auto lowerOk = (c1 > 0) || (fromInclusive && c1 == 0);
                const auto upperOk = (c2 < 0) || (toInclusive && c2 == 0);
                if (lowerOk && upperOk) out->addElement(x);
            }
            return out;
        }

        jxx::Ptr<NavigableSet<E>> descendingSet() const override
        {
            auto out = std::make_shared<TreeSet<E>>(comparator_);
            std::shared_lock<std::shared_mutex> lock(m_);
            for (auto it = set_.rbegin(); it != set_.rend(); ++it)
                out->addElement(*it);
            return out;
        }

        jxx::Ptr<jxx::lang::Object> clone() const
        {
            auto out = std::make_shared<TreeSet<E>>(comparator_);
            std::shared_lock<std::shared_mutex> lock(m_);
            for (const auto& x : set_) out->addElement(x);
            return out;
        }

        jxx::Ptr<jxx::lang::String> toString() const override
        {
            return std::make_shared<jxx::lang::String>(
                std::string("TreeSet[size=") + std::to_string(size()) + "]");
        }

    private:
        jxx::lang::jint compare_(jxx::Ptr<E> a, jxx::Ptr<E> b) const
        {
            if (!a && !b) return 0;
            if (!a) return -1;
            if (!b) return 1;
            if (comparator_) return comparator_->compare(a, b);
            auto ca = std::dynamic_pointer_cast<jxx::lang::Comparable<E>>(a);
            if (!ca)
                throw std::runtime_error("TreeSet element is not Comparable and no Comparator was supplied");
            return ca->compareTo(b);
        }

    private:
        mutable std::shared_mutex m_;
        jxx::Ptr<Comparator<E>> comparator_;
        NativeSet set_;
    };
}
