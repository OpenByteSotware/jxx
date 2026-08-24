#include "util/jxx.util.concurrent.TimeUnit.h"
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::util::concurrent
{
	jxx::lang::jlong TimeUnit::convert(jxx::lang::jlong duration, const jxx::Ptr<TimeUnit>& source) const
	{
		if (source == nullptr) throw jxx::lang::NullPointerException();
		const auto sourceFactor = factor(source->kind()); const auto targetFactor = factor(kind_);
		if (sourceFactor == targetFactor)return duration;
		if (sourceFactor < targetFactor)return duration / (targetFactor / sourceFactor);
		return saturatedMultiply(duration, sourceFactor / targetFactor);
	}
}
