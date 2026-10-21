#include <string>
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAccumulator.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.InvalidObjectException.h"

namespace jxx::util::concurrent::atomic
{
	::jxx::Ptr<::jxx::lang::ClassAny>LongAccumulator::Class()
	{
		return JxxClassInfoMarker::Class();
	}
	LongAccumulator::LongAccumulator() :Super(), function_(nullptr), identity_(0), value_(0)
	{
	}
	::jxx::Ptr<LongAccumulator> LongAccumulator::createForDeserialization()
	{
		return ::jxx::ADOPT<LongAccumulator>(::jxx::Ptr<LongAccumulator>(new LongAccumulator()));
	}
	LongAccumulator::LongAccumulator(const ::jxx::Ptr<::jxx::util::function::LongBinaryOperator>& f, ::jxx::lang::jlong i) :Super(), function_(f), identity_(i), value_(i)
	{
		if (!function_)throw ::jxx::lang::NullPointerException();
	}
	void LongAccumulator::accumulate(::jxx::lang::jlong x)
	{
		std::lock_guard<std::mutex>l(mutex_); value_ = function_->applyAsLong(value_, x);
	}::jxx::lang::jlong LongAccumulator::get()const
	{
		std::lock_guard<std::mutex>l(mutex_); return value_;
	}void LongAccumulator::reset()
	{
		std::lock_guard<std::mutex>l(mutex_); value_ = identity_;
	}::jxx::lang::jlong LongAccumulator::getThenReset()
	{
		std::lock_guard<std::mutex>l(mutex_); auto v = value_; value_ = identity_; return v;
	}
	::jxx::lang::jint LongAccumulator::intValue()const
	{
		return static_cast<::jxx::lang::jint>(get());
	}::jxx::lang::jlong LongAccumulator::longValue()const
	{
		return static_cast<::jxx::lang::jlong>(get());
	}::jxx::lang::jfloat LongAccumulator::floatValue()const
	{
		return static_cast<::jxx::lang::jfloat>(get());
	}::jxx::lang::jdouble LongAccumulator::doubleValue()const
	{
		return static_cast<::jxx::lang::jdouble>(get());
	}::jxx::Ptr<::jxx::lang::String>LongAccumulator::toString()const
	{
		return ::jxx::NEW<::jxx::lang::String>(std::to_string(get()));
	}void LongAccumulator::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out)
	{
		if (out == nullptr)throw ::jxx::lang::NullPointerException(); std::lock_guard<std::mutex> lock(mutex_); out->writeObject(::jxx::CAST<::jxx::lang::Object>(function_)); out->writeLong(identity_); out->writeLong(value_);
	}void LongAccumulator::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in)
	{
		if (in == nullptr)throw ::jxx::lang::NullPointerException();
		auto function = ::jxx::CAST<::jxx::util::function::LongBinaryOperator>(in->readObject()); 
		if (function == nullptr)throw ::jxx::io::InvalidObjectException("accumulator function");
		auto identity = in->readLong(); auto value = in->readLong(); std::lock_guard<std::mutex> lock(mutex_); function_ = function; identity_ = identity; value_ = value;
	}void LongAccumulator::readObjectNoData()
	{
		throw ::jxx::io::InvalidObjectException("missing accumulator state");
	}
}
