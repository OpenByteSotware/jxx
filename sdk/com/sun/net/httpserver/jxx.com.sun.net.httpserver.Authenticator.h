#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::com::sun::net::httpserver { class HttpExchange; class HttpPrincipal;
class Authenticator : public ::jxx::lang::ClassBase<Authenticator,::jxx::lang::Object> { public: using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Authenticator,JxxSuper>; class Result; class Success; class Failure; class Retry; ~Authenticator()override=default; virtual ::jxx::Ptr<Result> authenticate(const ::jxx::Ptr<HttpExchange>& exchange)=0; protected: Authenticator()=default;};
class Authenticator::Result:public ::jxx::lang::ClassBase<Result,::jxx::lang::Object>{public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<Result,JxxSuper>;~Result()override=default;protected:Result()=default;};
class Authenticator::Success final:public ::jxx::lang::ClassBase<Success,Result>{public:using JxxSuper=Result;using Super=::jxx::lang::ClassBase<Success,JxxSuper>;explicit Success(const ::jxx::Ptr<HttpPrincipal>& p);::jxx::Ptr<HttpPrincipal> getPrincipal()const;private: ::jxx::Ptr<HttpPrincipal> principal_;};
class Authenticator::Failure final:public ::jxx::lang::ClassBase<Failure,Result>{public:using JxxSuper=Result;using Super=::jxx::lang::ClassBase<Failure,JxxSuper>;explicit Failure(::jxx::lang::jint c);::jxx::lang::jint getResponseCode()const noexcept;private: ::jxx::lang::jint code_;};
class Authenticator::Retry final:public ::jxx::lang::ClassBase<Retry,Result>{public:using JxxSuper=Result;using Super=::jxx::lang::ClassBase<Retry,JxxSuper>;explicit Retry(::jxx::lang::jint c);::jxx::lang::jint getResponseCode()const noexcept;private: ::jxx::lang::jint code_;}; }
