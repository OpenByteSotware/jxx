#include "util/concurrent/locks/jxx.util.concurrent.locks.LockSupport.h"
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <unordered_map>
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::util::concurrent::locks { namespace {
struct Parker{std::mutex mutex;std::condition_variable condition;bool permit=false;::jxx::Ptr<::jxx::lang::Object> blocker;};
std::mutex registryMutex;std::unordered_map<::jxx::lang::jlong,std::shared_ptr<Parker>> registry;
std::shared_ptr<Parker> parker(::jxx::lang::jlong id){std::lock_guard<std::mutex>l(registryMutex);auto& p=registry[id];if(!p)p=std::make_shared<Parker>();return p;}
std::shared_ptr<Parker> current(){auto t=::jxx::lang::Thread::currentThread();if(!t)throw ::jxx::lang::IllegalStateException();return parker(t->getId());}
template<class Wait>void parkImpl(const ::jxx::Ptr<::jxx::lang::Object>& blocker,Wait wait){auto p=current();std::unique_lock<std::mutex>l(p->mutex);if(p->permit){p->permit=false;return;}p->blocker=blocker;wait(*p,l);p->blocker.reset();if(p->permit)p->permit=false;}
}
::jxx::Ptr<::jxx::lang::ClassAny> LockSupport::Class(){return JxxClassInfoMarker::Class();}
void LockSupport::unpark(const ::jxx::Ptr<::jxx::lang::Thread>&thread){if(!thread)return;auto p=parker(thread->getId());{std::lock_guard<std::mutex>l(p->mutex);p->permit=true;}p->condition.notify_one();}
void LockSupport::park(){park(nullptr);}void LockSupport::park(const ::jxx::Ptr<::jxx::lang::Object>&b){parkImpl(b,[] (Parker&p,std::unique_lock<std::mutex>&l){p.condition.wait(l,[&]{return p.permit;});});}
void LockSupport::parkNanos(::jxx::lang::jlong n){parkNanos(nullptr,n);}void LockSupport::parkNanos(const ::jxx::Ptr<::jxx::lang::Object>&b,::jxx::lang::jlong n){if(n<=0)return;parkImpl(b,[n](Parker&p,std::unique_lock<std::mutex>&l){p.condition.wait_for(l,std::chrono::nanoseconds(n),[&]{return p.permit;});});}
void LockSupport::parkUntil(::jxx::lang::jlong d){parkUntil(nullptr,d);}void LockSupport::parkUntil(const ::jxx::Ptr<::jxx::lang::Object>&b,::jxx::lang::jlong d){auto tp=std::chrono::system_clock::time_point(std::chrono::milliseconds(d));parkImpl(b,[tp](Parker&p,std::unique_lock<std::mutex>&l){p.condition.wait_until(l,tp,[&]{return p.permit;});});}
::jxx::Ptr<::jxx::lang::Object> LockSupport::getBlocker(const ::jxx::Ptr<::jxx::lang::Thread>&t){if(!t)throw ::jxx::lang::NullPointerException();auto p=parker(t->getId());std::lock_guard<std::mutex>l(p->mutex);return p->blocker;}
}
