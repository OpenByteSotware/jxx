#include "io/jxx.io.BufferedReader.h"
#include <algorithm>
#include <string>
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
namespace jxx::io
{
	BufferedReader::BufferedReader(const ::jxx::Ptr<Reader>& i) :BufferedReader(i, 8192)
	{
	} BufferedReader::BufferedReader(const ::jxx::Ptr<Reader>& i, ::jxx::lang::jint s) :in_(i)
	{
		if (!i)throw ::jxx::lang::NullPointerException(); if (s <= 0)throw ::jxx::lang::IllegalArgumentException(); buffer_ = ::jxx::NEW<::jxx::lang::CharArrayType>(static_cast<std::uint32_t>(s));
	} ::jxx::lang::jint BufferedReader::fill()
	{
		position_ = 0; count_ = in_->read(buffer_, 0, static_cast<::jxx::lang::jint>(buffer_->length)); return count_;
	} ::jxx::lang::jint BufferedReader::read()
	{
		if (position_ >= count_ && fill() < 0)return -1; return static_cast<::jxx::lang::jint>((*buffer_)[position_++]);
	} ::jxx::lang::jint BufferedReader::read(const ::jxx::lang::CharArray& b, ::jxx::lang::jint o, ::jxx::lang::jint l)
	{
		if (l == 0)return 0; ::jxx::lang::jint n = 0; while (n < l) {
			auto c = read(); if (c < 0)break; (*b)[o + n++] = static_cast<::jxx::lang::jchar>(c);
		}return n == 0 ? -1 : n;
	} ::jxx::Ptr<::jxx::lang::String> BufferedReader::readLine()
	{
		std::u16string line; for (;;) {
			auto c = read(); if (c < 0)if (line.empty()) {
				return nullptr;
			}
			return ::jxx::NEW<::jxx::lang::String>(line); if (c == '\n')break; if (c == '\r') {
				auto next = read(); if (next >= 0 && next != '\n')--position_; break;
			}line.push_back(static_cast<char16_t>(c));
		}return ::jxx::NEW<::jxx::lang::String>(line);
	} ::jxx::lang::jlong BufferedReader::skip(::jxx::lang::jlong n)
	{
		::jxx::lang::jlong total = 0; while (total < n && read() >= 0)++total; return total;
	} ::jxx::lang::jbool BufferedReader::ready()
	{
		return position_ < count_ || in_->ready();
	} ::jxx::lang::jbool BufferedReader::markSupported()const
	{
		return true;
	} void BufferedReader::mark(::jxx::lang::jint)
	{
		mark_ = position_;
	} void BufferedReader::reset()
	{
		if (mark_ < 0)throw IOException(); position_ = mark_;
	} void BufferedReader::close()
	{
		buffer_.reset(); in_->close(); in_.reset();
	}
}
