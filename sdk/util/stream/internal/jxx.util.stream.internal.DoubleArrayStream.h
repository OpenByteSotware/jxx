#pragma once
#include "lang/jxx.lang.Object.h"
#include "util/stream/jxx.util.stream.DoubleStream.h"
namespace jxx::util::stream::internal {
class DoubleArrayStream final : public ::jxx::lang::ClassBase<DoubleArrayStream, ::jxx::lang::Object, ::jxx::util::stream::DoubleStream> {
public:
 explicit DoubleArrayStream(const ::jxx::lang::DoubleArray& values);
 ::jxx::Ptr<::jxx::util::Iterator<::jxx::lang::Double>> iterator() override { return nullptr; }
 ::jxx::Ptr<::jxx::util::Spliterator<::jxx::lang::Double>> spliterator() override { return nullptr; }
 ::jxx::lang::jbool isParallel() override { return parallel_; }
 ::jxx::Ptr<::jxx::util::stream::DoubleStream> sequential() override;
 ::jxx::Ptr<::jxx::util::stream::DoubleStream> parallel() override;
 ::jxx::Ptr<::jxx::util::stream::DoubleStream> unordered() override;
 ::jxx::Ptr<::jxx::util::stream::DoubleStream> onClose(const ::jxx::Ptr<::jxx::lang::Runnable>& handler) override;
 void close() override;
 ::jxx::Ptr<::jxx::util::stream::DoubleStream> distinct() override;
 ::jxx::lang::DoubleArray toArray() override;
 ::jxx::lang::jlong count() override;
 ::jxx::lang::jdouble sum() override;
private:
 ::jxx::lang::DoubleArray data_; bool parallel_=false; bool closed_=false; ::jxx::Ptr<::jxx::lang::Runnable> closeHandler_;
};
}
