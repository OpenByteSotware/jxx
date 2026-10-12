#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::util::function {
template <typename T, typename R> class Function : public ::jxx::lang::InterfaceBase<Function<T,R>> {
public:
 ~Function() override=default;
 virtual ::jxx::Ptr<R> apply(const ::jxx::Ptr<T>& value)=0;
 template <typename V> ::jxx::Ptr<Function<V,R>> compose(const ::jxx::Ptr<Function<V,T>>& before);
 template <typename V> ::jxx::Ptr<Function<T,V>> andThen(const ::jxx::Ptr<Function<R,V>>& after);
 static ::jxx::Ptr<Function<T,T>> identity();
private: ::jxx::Ptr<Function<T,R>> self_();
};
template <typename V,typename T,typename R> class FunctionCompose final: public ::jxx::lang::ClassBase<FunctionCompose<V,T,R>,::jxx::lang::Object,Function<V,R>> { public: FunctionCompose(const ::jxx::Ptr<Function<V,T>>& a,const ::jxx::Ptr<Function<T,R>>& b):a_(a),b_(b){} ::jxx::Ptr<R> apply(const ::jxx::Ptr<V>& v) override{return b_->apply(a_->apply(v));} private: ::jxx::Ptr<Function<V,T>> a_; ::jxx::Ptr<Function<T,R>> b_;};
template <typename T,typename R,typename V> class FunctionAndThen final: public ::jxx::lang::ClassBase<FunctionAndThen<T,R,V>,::jxx::lang::Object,Function<T,V>> { public: FunctionAndThen(const ::jxx::Ptr<Function<T,R>>& a,const ::jxx::Ptr<Function<R,V>>& b):a_(a),b_(b){} ::jxx::Ptr<V> apply(const ::jxx::Ptr<T>& v) override{return b_->apply(a_->apply(v));} private: ::jxx::Ptr<Function<T,R>> a_; ::jxx::Ptr<Function<R,V>> b_;};
template <typename T> class IdentityFunction final: public ::jxx::lang::ClassBase<IdentityFunction<T>,::jxx::lang::Object,Function<T,T>> { public: ::jxx::Ptr<T> apply(const ::jxx::Ptr<T>& v) override{return v;} };
template <typename T,typename R> ::jxx::Ptr<Function<T,R>> Function<T,R>::self_(){auto o=dynamic_cast<::jxx::lang::Object*>(this);if(!o)throw ::jxx::lang::IllegalStateException();auto s=std::dynamic_pointer_cast<Function<T,R>>(o->thisPtr());if(!s)throw ::jxx::lang::IllegalStateException();return s;}
template <typename T,typename R> template <typename V> ::jxx::Ptr<Function<V,R>> Function<T,R>::compose(const ::jxx::Ptr<Function<V,T>>& before){if(!before)throw ::jxx::lang::NullPointerException();auto x=::jxx::NEW<FunctionCompose<V,T,R>>(before,self_());return std::static_pointer_cast<Function<V,R>>(x);}
template <typename T,typename R> template <typename V> ::jxx::Ptr<Function<T,V>> Function<T,R>::andThen(const ::jxx::Ptr<Function<R,V>>& after){if(!after)throw ::jxx::lang::NullPointerException();auto x=::jxx::NEW<FunctionAndThen<T,R,V>>(self_(),after);return std::static_pointer_cast<Function<T,V>>(x);}
template <typename T,typename R> ::jxx::Ptr<Function<T,T>> Function<T,R>::identity(){auto x=::jxx::NEW<IdentityFunction<T>>();return std::static_pointer_cast<Function<T,T>>(x);}
} // namespace jxx::util::function
