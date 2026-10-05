#pragma once
#include <type_traits>
#include <utility>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
template<typename F> class JxxScheduledTestRunnable final : public ::jxx::lang::ClassBase<JxxScheduledTestRunnable<F>,::jxx::lang::Object,::jxx::lang::Runnable>{public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<JxxScheduledTestRunnable<F>,JxxSuper,::jxx::lang::Runnable>;explicit JxxScheduledTestRunnable(F f):Super(),f_(std::move(f)){}void run()override{f_();}private:F f_;};
template<typename F> auto jxxScheduledTestRunnable(F&& f){using T=JxxScheduledTestRunnable<typename std::decay<F>::type>;return ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<T>(std::forward<F>(f)));}
