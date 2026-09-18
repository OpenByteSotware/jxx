#include "util/jxx.util.concurrent.Phaser.h"

#include <limits>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.concurrent.TimeoutException.h"

namespace jxx::util::concurrent {
::jxx::Ptr<::jxx::lang::ClassAny> Phaser::Class(){return JxxClassInfoMarker::Class();}
Phaser::Phaser():Phaser(nullptr,0){}
Phaser::Phaser(::jxx::lang::jint parties):Phaser(nullptr,parties){}
Phaser::Phaser(const ::jxx::Ptr<Phaser>& parent):Phaser(parent,0){}
Phaser::Phaser(const ::jxx::Ptr<Phaser>& parent,::jxx::lang::jint parties):Super(),parent_(parent),registered_(parties),unarrived_(parties){if(parties<0)throw ::jxx::lang::IllegalArgumentException();}
::jxx::lang::jint Phaser::registerParty(){return bulkRegister(1);}
::jxx::lang::jint Phaser::bulkRegister(::jxx::lang::jint parties){if(parties<0)throw ::jxx::lang::IllegalArgumentException();std::lock_guard<std::mutex>l(mutex_);if(terminated_)return -1;registered_+=parties;unarrived_+=parties;return phase_;}
::jxx::lang::jint Phaser::arrive(){return doArrive_(false);}
::jxx::lang::jint Phaser::arriveAndDeregister(){return doArrive_(true);}
::jxx::lang::jint Phaser::doArrive_(::jxx::lang::jbool deregister){std::lock_guard<std::mutex>l(mutex_);if(terminated_)return -1;if(unarrived_<=0)throw ::jxx::lang::IllegalStateException();const auto result=phase_;--unarrived_;if(deregister)--registered_;if(unarrived_==0){const auto terminate=onAdvance(phase_,registered_);phase_=phase_==std::numeric_limits<::jxx::lang::jint>::max()?0:phase_+1;unarrived_=registered_;terminated_=terminate;condition_.notify_all();}return result;}
::jxx::lang::jint Phaser::arriveAndAwaitAdvance(){const auto p=arrive();return awaitAdvance(p);}
::jxx::lang::jint Phaser::awaitAdvance(::jxx::lang::jint p){std::unique_lock<std::mutex>l(mutex_);condition_.wait(l,[&]{return terminated_||phase_!=p;});return terminated_?-1:phase_;}
::jxx::lang::jint Phaser::awaitAdvanceInterruptibly(::jxx::lang::jint p){return awaitAdvance(p);}
::jxx::lang::jint Phaser::awaitAdvanceInterruptibly(::jxx::lang::jint p,::jxx::lang::jlong timeout,const ::jxx::Ptr<TimeUnit>&unit){if(!unit)throw ::jxx::lang::NullPointerException();std::unique_lock<std::mutex>l(mutex_);if(!condition_.wait_for(l,unit->toChrono(timeout),[&]{return terminated_||phase_!=p;}))throw TimeoutException();return terminated_?-1:phase_;}
void Phaser::forceTermination(){std::lock_guard<std::mutex>l(mutex_);terminated_=true;condition_.notify_all();}
::jxx::lang::jbool Phaser::isTerminated()const{std::lock_guard<std::mutex>l(mutex_);return terminated_;}
::jxx::lang::jint Phaser::getPhase()const{std::lock_guard<std::mutex>l(mutex_);return terminated_?-1:phase_;}
::jxx::lang::jint Phaser::getRegisteredParties()const{std::lock_guard<std::mutex>l(mutex_);return registered_;}
::jxx::lang::jint Phaser::getArrivedParties()const{std::lock_guard<std::mutex>l(mutex_);return registered_-unarrived_;}
::jxx::lang::jint Phaser::getUnarrivedParties()const{std::lock_guard<std::mutex>l(mutex_);return unarrived_;}
::jxx::Ptr<Phaser> Phaser::getParent()const{return parent_;}
::jxx::Ptr<Phaser> Phaser::getRoot()const{auto root=::jxx::CAST<Phaser>(const_cast<Phaser*>(this)->thisPtr());while(root->parent_!=nullptr)root=root->parent_;return root;}
::jxx::lang::jbool Phaser::onAdvance(::jxx::lang::jint,::jxx::lang::jint parties){return parties==0;}
} // namespace jxx::util::concurrent
