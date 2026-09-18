#pragma once
#include <condition_variable>
#include <mutex>
#include <vector>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.AbstractQueue.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.concurrent.BlockingQueue.h"
namespace jxx::util::concurrent {
template<typename E> class SynchronousQueue final : public ::jxx::lang::ClassBase<SynchronousQueue<E>,::jxx::util::AbstractQueue<E>,BlockingQueue<E>,::jxx::io::SerializableI> {
 class EmptyIterator final : public ::jxx::lang::ClassBase<EmptyIterator,::jxx::lang::Object,::jxx::util::Iterator<E>> { public: ::jxx::lang::jbool hasNext() override{return false;} ::jxx::Ptr<E> next() override{throw ::jxx::util::NoSuchElementException();} void remove() override{throw ::jxx::lang::UnsupportedOperationException();} };
public:
 using JxxSuper=::jxx::util::AbstractQueue<E>; using Super=::jxx::lang::ClassBase<SynchronousQueue<E>,JxxSuper,BlockingQueue<E>,::jxx::io::SerializableI>; using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class(){return JxxClassInfoMarker::Class();}
 SynchronousQueue():SynchronousQueue(false){} explicit SynchronousQueue(::jxx::lang::jbool fair):Super(),fair_(fair){}
 ::jxx::lang::jbool offer(const ::jxx::Ptr<E>& e) override { require_(e); std::unique_lock<std::mutex> l(m_); if(waitingConsumers_==0||slot_) return false; slot_=e; cv_.notify_all(); cv_.wait(l,[&]{return !slot_;}); return true; }
 void put(const ::jxx::Ptr<E>& e) override { require_(e); std::unique_lock<std::mutex> l(m_); cv_.wait(l,[&]{return waitingConsumers_>0&&!slot_;}); slot_=e; cv_.notify_all(); cv_.wait(l,[&]{return !slot_;}); }
 ::jxx::lang::jbool offer(const ::jxx::Ptr<E>& e,::jxx::lang::jlong t,const ::jxx::Ptr<TimeUnit>& u) override {require_(e);if(!u)throw ::jxx::lang::NullPointerException();std::unique_lock<std::mutex>l(m_);auto d=std::chrono::steady_clock::now()+u->toChrono(t);if(!cv_.wait_until(l,d,[&]{return waitingConsumers_>0&&!slot_;}))return false;slot_=e;cv_.notify_all();if(!cv_.wait_until(l,d,[&]{return !slot_;})){slot_.reset();cv_.notify_all();return false;}return true;}
 ::jxx::Ptr<E> take() override {std::unique_lock<std::mutex>l(m_);++waitingConsumers_;cv_.notify_all();cv_.wait(l,[&]{return slot_!=nullptr;});auto r=slot_;slot_.reset();--waitingConsumers_;cv_.notify_all();return r;}
 ::jxx::Ptr<E> poll() override {std::lock_guard<std::mutex>l(m_);if(!slot_)return nullptr;auto r=slot_;slot_.reset();cv_.notify_all();return r;}
 ::jxx::Ptr<E> poll(::jxx::lang::jlong t,const ::jxx::Ptr<TimeUnit>&u) override {if(!u)throw ::jxx::lang::NullPointerException();std::unique_lock<std::mutex>l(m_);++waitingConsumers_;cv_.notify_all();if(!cv_.wait_for(l,u->toChrono(t),[&]{return slot_!=nullptr;})){--waitingConsumers_;return nullptr;}auto r=slot_;slot_.reset();--waitingConsumers_;cv_.notify_all();return r;}
 ::jxx::Ptr<E> peek() override{return nullptr;} ::jxx::lang::jint size() override{return 0;} ::jxx::lang::jint remainingCapacity() override{return 0;} void clear() override{} ::jxx::lang::jbool remove(const ::jxx::Ptr<::jxx::lang::Object>&) override{return false;}
 ::jxx::Ptr<::jxx::util::Iterator<E>> iterator() override{return ::jxx::CAST<::jxx::util::Iterator<E>>(::jxx::NEW<EmptyIterator>());}
 ::jxx::lang::jint drainTo(const ::jxx::Ptr<::jxx::util::Collection<E>>& c) override{return drainTo(c,1);} ::jxx::lang::jint drainTo(const ::jxx::Ptr<::jxx::util::Collection<E>>&c,::jxx::lang::jint n) override{if(!c)throw ::jxx::lang::NullPointerException();if(n<=0)return 0;auto e=poll();if(!e)return 0;c->add(e);return 1;}
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o)override{(void)o;}void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i)override{(void)i;}void readObjectNoData()override{}
private: static void require_(const ::jxx::Ptr<E>&e){if(!e)throw ::jxx::lang::NullPointerException();} std::mutex m_;std::condition_variable cv_;::jxx::Ptr<E>slot_;::jxx::lang::jint waitingConsumers_=0;::jxx::lang::jbool fair_;
}; }
