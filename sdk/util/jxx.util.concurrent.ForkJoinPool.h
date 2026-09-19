#pragma once
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::util::concurrent {
class ForkJoinPool final:public ::jxx::lang::ClassBase<ForkJoinPool,::jxx::lang::Object>{
public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<ForkJoinPool,JxxSuper>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<ForkJoinPool,JxxSuper>;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}explicit ForkJoinPool(::jxx::lang::jint parallelism=static_cast<::jxx::lang::jint>(std::thread::hardware_concurrency())):Super(),parallelism_(std::max<::jxx::lang::jint>(1,parallelism)){}static ::jxx::Ptr<ForkJoinPool>commonPool(){static auto pool=::jxx::NEW<ForkJoinPool>();return pool;}::jxx::lang::jint getParallelism()const noexcept{return parallelism_;}
 template<typename F>void parallelFor(std::size_t size,::jxx::lang::jlong threshold,F function)const{if(size==0)return;if(threshold<=0||size<static_cast<std::size_t>(threshold)||parallelism_<=1){function(0,size);return;}const auto tasks=std::min<std::size_t>(size,static_cast<std::size_t>(parallelism_));const auto chunk=(size+tasks-1)/tasks;std::vector<std::future<void>>futures;for(std::size_t begin=0;begin<size;begin+=chunk){const auto end=std::min(size,begin+chunk);futures.emplace_back(std::async(std::launch::async,[=,&function]{function(begin,end);}));}for(auto&future:futures)future.get();}
 template<typename R,typename F,typename Reduce>R parallelReduce(std::size_t size,::jxx::lang::jlong threshold,R identity,F function,Reduce reducer)const{if(size==0)return identity;if(threshold<=0||size<static_cast<std::size_t>(threshold)||parallelism_<=1)return function(0,size);const auto tasks=std::min<std::size_t>(size,static_cast<std::size_t>(parallelism_));const auto chunk=(size+tasks-1)/tasks;std::vector<std::future<R>>futures;for(std::size_t begin=0;begin<size;begin+=chunk){const auto end=std::min(size,begin+chunk);futures.emplace_back(std::async(std::launch::async,[=,&function]{return function(begin,end);}));}auto result=identity;for(auto&future:futures)result=reducer(result,future.get());return result;}
private: ::jxx::lang::jint parallelism_;};}
