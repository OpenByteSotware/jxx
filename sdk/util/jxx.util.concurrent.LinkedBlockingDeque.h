#pragma once

#include <condition_variable>
#include <deque>
#include <limits>
#include <mutex>
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.AbstractQueue.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.concurrent.BlockingDeque.h"

namespace jxx::util::concurrent {

template<typename E>
class LinkedBlockingDeque final
    : public ::jxx::lang::ClassBase<LinkedBlockingDeque<E>,
          ::jxx::util::AbstractQueue<E>, BlockingDeque<E>,
          ::jxx::io::SerializableI> {
private:
    class SnapshotIterator final
        : public ::jxx::lang::ClassBase<SnapshotIterator,
              ::jxx::lang::Object, ::jxx::util::Iterator<E>> {
    public:
        explicit SnapshotIterator(std::vector<::jxx::Ptr<E>> values)
            : Super(), values_(std::move(values)) {}
        ::jxx::lang::jbool hasNext() override { return index_ < values_.size(); }
        ::jxx::Ptr<E> next() override {
            if (!hasNext()) throw ::jxx::util::NoSuchElementException();
            return values_[index_++];
        }
        void remove() override { throw ::jxx::lang::UnsupportedOperationException(); }
    private:
        using Super = ::jxx::lang::ClassBase<SnapshotIterator,
            ::jxx::lang::Object, ::jxx::util::Iterator<E>>;
        std::vector<::jxx::Ptr<E>> values_;
        std::size_t index_ = 0;
    };

public:
    using JxxSuper = ::jxx::util::AbstractQueue<E>;
    using Super = ::jxx::lang::ClassBase<LinkedBlockingDeque<E>,
        JxxSuper, BlockingDeque<E>, ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() { return JxxClassInfoMarker::Class(); }

    LinkedBlockingDeque()
        : LinkedBlockingDeque(std::numeric_limits<::jxx::lang::jint>::max()) {}
    explicit LinkedBlockingDeque(::jxx::lang::jint capacity)
        : Super(), capacity_(capacity) {
        if (capacity <= 0) throw ::jxx::lang::IllegalArgumentException();
    }

    void addFirst(const ::jxx::Ptr<E>& e) override { if (!offerFirst(e)) throw ::jxx::lang::IllegalStateException(); }
    void addLast(const ::jxx::Ptr<E>& e) override { if (!offerLast(e)) throw ::jxx::lang::IllegalStateException(); }
    ::jxx::lang::jbool offerFirst(const ::jxx::Ptr<E>& e) override { return offerEnd_(e, true); }
    ::jxx::lang::jbool offerLast(const ::jxx::Ptr<E>& e) override { return offerEnd_(e, false); }
    void putFirst(const ::jxx::Ptr<E>& e) override { putEnd_(e, true); }
    void putLast(const ::jxx::Ptr<E>& e) override { putEnd_(e, false); }
    ::jxx::lang::jbool offerFirst(const ::jxx::Ptr<E>& e, ::jxx::lang::jlong t,
        const ::jxx::Ptr<TimeUnit>& u) override { return timedOffer_(e,t,u,true); }
    ::jxx::lang::jbool offerLast(const ::jxx::Ptr<E>& e, ::jxx::lang::jlong t,
        const ::jxx::Ptr<TimeUnit>& u) override { return timedOffer_(e,t,u,false); }

    ::jxx::Ptr<E> removeFirst() override { auto e=pollFirst(); if(!e) throw ::jxx::util::NoSuchElementException(); return e; }
    ::jxx::Ptr<E> removeLast() override { auto e=pollLast(); if(!e) throw ::jxx::util::NoSuchElementException(); return e; }
    ::jxx::Ptr<E> pollFirst() override { return pollEnd_(true); }
    ::jxx::Ptr<E> pollLast() override { return pollEnd_(false); }
    ::jxx::Ptr<E> takeFirst() override { return takeEnd_(true); }
    ::jxx::Ptr<E> takeLast() override { return takeEnd_(false); }
    ::jxx::Ptr<E> pollFirst(::jxx::lang::jlong t,const ::jxx::Ptr<TimeUnit>&u) override { return timedPoll_(t,u,true); }
    ::jxx::Ptr<E> pollLast(::jxx::lang::jlong t,const ::jxx::Ptr<TimeUnit>&u) override { return timedPoll_(t,u,false); }
    ::jxx::Ptr<E> getFirst() override { auto e=peekFirst(); if(!e) throw ::jxx::util::NoSuchElementException(); return e; }
    ::jxx::Ptr<E> getLast() override { auto e=peekLast(); if(!e) throw ::jxx::util::NoSuchElementException(); return e; }
    ::jxx::Ptr<E> peekFirst() override { std::lock_guard<std::mutex> l(m_); return q_.empty()?nullptr:q_.front(); }
    ::jxx::Ptr<E> peekLast() override { std::lock_guard<std::mutex> l(m_); return q_.empty()?nullptr:q_.back(); }

    ::jxx::lang::jbool offer(const ::jxx::Ptr<E>& e) override { return offerLast(e); }
    void put(const ::jxx::Ptr<E>& e) override { putLast(e); }
    ::jxx::lang::jbool offer(const ::jxx::Ptr<E>&e,::jxx::lang::jlong t,const ::jxx::Ptr<TimeUnit>&u) override{return offerLast(e,t,u);}
    ::jxx::Ptr<E> poll() override { return pollFirst(); }
    ::jxx::Ptr<E> take() override { return takeFirst(); }
    ::jxx::Ptr<E> poll(::jxx::lang::jlong t,const ::jxx::Ptr<TimeUnit>&u) override{return pollFirst(t,u);}
    ::jxx::Ptr<E> peek() override { return peekFirst(); }
    void push(const ::jxx::Ptr<E>& e) override { addFirst(e); }
    ::jxx::Ptr<E> pop() override { return removeFirst(); }

    ::jxx::lang::jbool removeFirstOccurrence(const ::jxx::Ptr<::jxx::lang::Object>&o) override{return removeOccurrence_(o,true);}
    ::jxx::lang::jbool removeLastOccurrence(const ::jxx::Ptr<::jxx::lang::Object>&o) override{return removeOccurrence_(o,false);}
    ::jxx::lang::jbool remove(const ::jxx::Ptr<::jxx::lang::Object>&o) override{return removeFirstOccurrence(o);}
    ::jxx::lang::jint size() override { std::lock_guard<std::mutex> l(m_); return static_cast<::jxx::lang::jint>(q_.size()); }
    ::jxx::lang::jint remainingCapacity() override { std::lock_guard<std::mutex> l(m_); return capacity_-static_cast<::jxx::lang::jint>(q_.size()); }
    void clear() override { std::lock_guard<std::mutex> l(m_); q_.clear(); notFull_.notify_all(); }
    ::jxx::Ptr<::jxx::util::Iterator<E>> iterator() override { return snapshot_(false); }
    ::jxx::Ptr<::jxx::util::Iterator<E>> descendingIterator() override { return snapshot_(true); }
    ::jxx::lang::jint drainTo(const ::jxx::Ptr<::jxx::util::Collection<E>>&c) override{return drainTo(c,std::numeric_limits<::jxx::lang::jint>::max());}
    ::jxx::lang::jint drainTo(const ::jxx::Ptr<::jxx::util::Collection<E>>&c,::jxx::lang::jint n) override {
        if(!c)throw ::jxx::lang::NullPointerException(); if(n<=0)return 0;
        std::lock_guard<std::mutex> l(m_); ::jxx::lang::jint count=0;
        while(!q_.empty()&&count<n){c->add(q_.front());q_.pop_front();++count;}
        if(count)notFull_.notify_all(); return count;
    }
    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o)override{(void)o;}
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i)override{(void)i;}
    void readObjectNoData()override{}

private:
    static void require_(const ::jxx::Ptr<E>&e){if(!e)throw ::jxx::lang::NullPointerException();}
    ::jxx::lang::jbool offerEnd_(const ::jxx::Ptr<E>&e,bool first){require_(e);std::lock_guard<std::mutex>l(m_);if(q_.size()>=static_cast<std::size_t>(capacity_))return false;first?q_.push_front(e):q_.push_back(e);notEmpty_.notify_one();return true;}
    void putEnd_(const ::jxx::Ptr<E>&e,bool first){require_(e);std::unique_lock<std::mutex>l(m_);notFull_.wait(l,[&]{return q_.size()<static_cast<std::size_t>(capacity_);});first?q_.push_front(e):q_.push_back(e);notEmpty_.notify_one();}
    ::jxx::lang::jbool timedOffer_(const ::jxx::Ptr<E>&e,::jxx::lang::jlong t,const ::jxx::Ptr<TimeUnit>&u,bool first){require_(e);if(!u)throw ::jxx::lang::NullPointerException();std::unique_lock<std::mutex>l(m_);if(!notFull_.wait_for(l,u->toChrono(t),[&]{return q_.size()<static_cast<std::size_t>(capacity_);}))return false;first?q_.push_front(e):q_.push_back(e);notEmpty_.notify_one();return true;}
    ::jxx::Ptr<E> pollEnd_(bool first){std::lock_guard<std::mutex>l(m_);if(q_.empty())return nullptr;auto e=first?q_.front():q_.back();if(first)q_.pop_front();else q_.pop_back();notFull_.notify_one();return e;}
    ::jxx::Ptr<E> takeEnd_(bool first){std::unique_lock<std::mutex>l(m_);notEmpty_.wait(l,[&]{return !q_.empty();});auto e=first?q_.front():q_.back();if(first)q_.pop_front();else q_.pop_back();notFull_.notify_one();return e;}
    ::jxx::Ptr<E> timedPoll_(::jxx::lang::jlong t,const ::jxx::Ptr<TimeUnit>&u,bool first){if(!u)throw ::jxx::lang::NullPointerException();std::unique_lock<std::mutex>l(m_);if(!notEmpty_.wait_for(l,u->toChrono(t),[&]{return !q_.empty();}))return nullptr;auto e=first?q_.front():q_.back();if(first)q_.pop_front();else q_.pop_back();notFull_.notify_one();return e;}
    ::jxx::lang::jbool removeOccurrence_(const ::jxx::Ptr<::jxx::lang::Object>&o,bool first){if(!o)return false;std::lock_guard<std::mutex>l(m_);if(first){for(auto i=q_.begin();i!=q_.end();++i)if((*i)->equals(o)){q_.erase(i);notFull_.notify_one();return true;}}else{for(auto i=q_.rbegin();i!=q_.rend();++i)if((*i)->equals(o)){q_.erase(std::next(i).base());notFull_.notify_one();return true;}}return false;}
    ::jxx::Ptr<::jxx::util::Iterator<E>> snapshot_(bool reverse){std::vector<::jxx::Ptr<E>>v;{std::lock_guard<std::mutex>l(m_);v.assign(q_.begin(),q_.end());}if(reverse)std::reverse(v.begin(),v.end());return ::jxx::CAST<::jxx::util::Iterator<E>>(::jxx::NEW<SnapshotIterator>(std::move(v)));}
    std::mutex m_; std::condition_variable notEmpty_,notFull_; std::deque<::jxx::Ptr<E>> q_; ::jxx::lang::jint capacity_;
};
} // namespace jxx::util::concurrent
