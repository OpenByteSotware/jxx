#include "io/jxx.io.StringReader.h"
#include <algorithm>
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::io
{
	StringReader::StringReader(const ::jxx::Ptr<::jxx::lang::String>& v) :value_(v)
	{
		if (!v)throw ::jxx::lang::NullPointerException();
	} void StringReader::ensureOpen()const
	{
		if (!value_)throw IOException();
	} ::jxx::lang::jint StringReader::read()
	{
		ensureOpen(); return position_ >= value_->length() ? -1 : static_cast<::jxx::lang::jint>(value_->charAt(position_++));
	} ::jxx::lang::jint StringReader::read(const ::jxx::lang::CharArray& b, ::jxx::lang::jint o, ::jxx::lang::jint l)
	{
		ensureOpen(); if (!b)throw ::jxx::lang::NullPointerException(); if (o < 0 || l<0 || o>static_cast<::jxx::lang::jint>(b->length) - l)throw ::jxx::lang::IndexOutOfBoundsException(); if (l == 0)return 0; if (position_ >= value_->length())return -1; auto n = std::min(l, value_->length() - position_); for (::jxx::lang::jint i = 0; i < n; ++i)(*b)[o + i] = value_->charAt(position_ + i); position_ += n; return n;
	} ::jxx::lang::jlong StringReader::skip(::jxx::lang::jlong n)
	{
		ensureOpen(); auto k = std::max<::jxx::lang::jlong>(-position_, std::min<::jxx::lang::jlong>(n, value_->length() - position_)); position_ += static_cast<::jxx::lang::jint>(k); return k;
	} ::jxx::lang::jbool StringReader::ready()
	{
		ensureOpen(); return true;
	} ::jxx::lang::jbool StringReader::markSupported()const
	{
		return true;
	} void StringReader::mark(::jxx::lang::jint l)
	{
		ensureOpen(); if (l < 0)throw ::jxx::lang::IllegalArgumentException(); mark_ = position_;
	} void StringReader::reset()
	{
		ensureOpen(); position_ = mark_;
	} void StringReader::close()
	{
		value_.reset();
	}
}
