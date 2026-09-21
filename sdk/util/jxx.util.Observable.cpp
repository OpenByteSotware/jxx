#include "util/jxx.util.Observable.h"

#include <algorithm>

#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.Observer.h"

namespace jxx::util {

Observable::Observable() : Super() {}
Observable::~Observable() = default;

::jxx::lang::jbool Observable::sameObserver_(
    const ::jxx::Ptr<Observer>& left,
    const ::jxx::Ptr<Observer>& right) {
    if (left == right) return true;
    if (left == nullptr || right == nullptr) return false;
    const auto leftObject = ::jxx::CAST<::jxx::lang::Object>(left);
    const auto rightObject = ::jxx::CAST<::jxx::lang::Object>(right);
    return leftObject != nullptr && rightObject != nullptr &&
        leftObject->equals(rightObject);
}

void Observable::addObserver(
    const ::jxx::Ptr<Observer>& observer) {
    if (observer == nullptr) throw ::jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> lock(observerMutex_);
    const auto found = std::find_if(
        observers_.begin(), observers_.end(),
        [&](const ::jxx::Ptr<Observer>& current) {
            return sameObserver_(current, observer);
        });
    if (found == observers_.end()) observers_.push_back(observer);
}

void Observable::deleteObserver(
    const ::jxx::Ptr<Observer>& observer) {
    std::lock_guard<std::mutex> lock(observerMutex_);
    const auto found = std::find_if(
        observers_.begin(), observers_.end(),
        [&](const ::jxx::Ptr<Observer>& current) {
            return sameObserver_(current, observer);
        });
    if (found != observers_.end()) observers_.erase(found);
}

void Observable::notifyObservers() {
    notifyObservers(nullptr);
}

void Observable::notifyObservers(
    const ::jxx::Ptr<::jxx::lang::Object>& argument) {
    std::vector<::jxx::Ptr<Observer>> snapshot;
    {
        std::lock_guard<std::mutex> lock(observerMutex_);
        if (!changed_) return;
        snapshot = observers_;
        changed_ = false;
    }

    const auto self = ::jxx::CAST<Observable>(thisPtr());
    for (const auto& observer : snapshot) {
        observer->update(self, argument);
    }
}

void Observable::deleteObservers() {
    std::lock_guard<std::mutex> lock(observerMutex_);
    observers_.clear();
}

::jxx::lang::jbool Observable::hasChanged() const {
    std::lock_guard<std::mutex> lock(observerMutex_);
    return changed_;
}

::jxx::lang::jint Observable::countObservers() const {
    std::lock_guard<std::mutex> lock(observerMutex_);
    return static_cast<::jxx::lang::jint>(observers_.size());
}

void Observable::setChanged() {
    std::lock_guard<std::mutex> lock(observerMutex_);
    changed_ = true;
}

void Observable::clearChanged() {
    std::lock_guard<std::mutex> lock(observerMutex_);
    changed_ = false;
}

} // namespace jxx::util
