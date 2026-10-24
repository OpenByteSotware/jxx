#pragma once
#include "lang/jxx.lang.Object.h"
#include "util/stream/jxx.util.stream.LongStream.h"
namespace jxx::util::stream::internal {
class LongArrayStream final : public ::jxx::lang::ClassBase<LongArrayStream, ::jxx::lang::Object, ::jxx::util::stream::LongStream> {
public:
 explicit LongArrayStream(const ::jxx::lang::LongArray& values);
 ::jxx::Ptr<::jxx::util::Iterator<::jxx::lang::Long>> iterator() override { return nullptr; }
 ::jxx::Ptr<::jxx::util::Spliterator<::jxx::lang::Long>> spliterator() override { return nullptr; }
 ::jxx::lang::jbool isParallel() override { return parallel_; }
 ::jxx::Ptr<::jxx::util::stream::LongStream> sequential() override;
 ::jxx::Ptr<::jxx::util::stream::LongStream> parallel() override;
 ::jxx::Ptr<::jxx::util::stream::LongStream> unordered() override;
 ::jxx::Ptr<::jxx::util::stream::LongStream> onClose(const ::jxx::Ptr<::jxx::lang::Runnable>& handler) override;
 void close() override;
 ::jxx::Ptr<::jxx::util::stream::LongStream> distinct() override;
 ::jxx::lang::LongArray toArray() override;
 ::jxx::lang::jlong count() override;
 ::jxx::lang::jlong sum() override;
private:
 ::jxx::lang::LongArray data_; bool parallel_=false; bool closed_=false; ::jxx::Ptr<::jxx::lang::Runnable> closeHandler_;
};
}
