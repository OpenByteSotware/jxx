#pragma once

#include <mutex>
#include <vector>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::util {

class Observer;

class Observable
    : public ::jxx::lang::ClassBase<
          Observable,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Observable, JxxSuper>;

    Observable();
    ~Observable() override;

    void addObserver(const ::jxx::Ptr<Observer>& observer);
    void deleteObserver(const ::jxx::Ptr<Observer>& observer);
    void notifyObservers();
    void notifyObservers(
        const ::jxx::Ptr<::jxx::lang::Object>& argument);
    void deleteObservers();
    ::jxx::lang::jbool hasChanged() const;
    ::jxx::lang::jint countObservers() const;

protected:
    void setChanged();
    void clearChanged();

private:
    static ::jxx::lang::jbool sameObserver_(
        const ::jxx::Ptr<Observer>& left,
        const ::jxx::Ptr<Observer>& right);

    mutable std::mutex observerMutex_;
    std::vector<::jxx::Ptr<Observer>> observers_;
    ::jxx::lang::jbool changed_ = false;
};

} // namespace jxx::util
