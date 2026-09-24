#include "com/google/gson/internal/jxx.com.google.gson.internal.StringWriter.h"
namespace com::google::gson::internal
{
	void StringWriter::write(const ::jxx::lang::CharArray& b, ::jxx::lang::jint o, ::jxx::lang::jint n)
	{
		for (::jxx::lang::jint i = 0; i < n; ++i)value_.push_back(static_cast<char>((*b)[o + i]));
	}
	void StringWriter::write(const ::jxx::Ptr<::jxx::lang::String>& v)
	{
		if (v != nullptr)value_ += v->utf8();
	}
	::jxx::Ptr<::jxx::lang::String> StringWriter::toString() const
	{
		return ::jxx::NEW<::jxx::lang::String>(value_);
	}
void StringWriter::flush() {}
void StringWriter::close() {}
}
