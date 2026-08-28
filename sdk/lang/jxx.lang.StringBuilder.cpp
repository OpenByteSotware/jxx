#include "lang/jxx.lang.StringBuilder.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <locale>
#include <sstream>
#include <utility>

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.StringBuffer.h"

namespace jxx::lang
{
	namespace
	{
		std::u16string charArrayText(const CharArray& array, jint offset, jint length)
		{
			std::u16string result;
			result.reserve(static_cast<std::size_t>(length));
			for (jint i = 0; i < length; ++i) result.push_back(static_cast<char16_t>((*array)[offset + i]));
			return result;
		}
	}

	bool StringBuilder::isHigh_(char16_t v) noexcept
	{
		return v >= 0xD800 && v <= 0xDBFF;
	}
	bool StringBuilder::isLow_(char16_t v) noexcept
	{
		return v >= 0xDC00 && v <= 0xDFFF;
	}
	void StringBuilder::throwNPE_()
	{
		throw NullPointerException();
	}
	void StringBuilder::throwSIOOBE_()
	{
		throw StringIndexOutOfBoundsException();
	}
	void StringBuilder::throwIAE_(const char*)
	{
		throw IllegalArgumentException();
	}

	jxx::Ptr<StringBuilder> StringBuilder::self_()
	{
		return jxx::CAST<StringBuilder>(thisPtr);
	}

	StringBuilder::StringBuilder() : value_(), capacity_(DEFAULT_CAPACITY)
	{
		value_.reserve(capacity_);
	}
	StringBuilder::StringBuilder(jint capacity) : value_(), capacity_(capacity)
	{
		if (capacity < 0) throw NegativeArraySizeException();
		value_.reserve(static_cast<std::size_t>(capacity_));
	}
	StringBuilder::StringBuilder(const jxx::Ptr<String>& string) : value_(), capacity_(0)
	{
		if (string == nullptr) throwNPE_();
		value_ = string->utf16();
		capacity_ = static_cast<jint>(value_.size()) + DEFAULT_CAPACITY;
		value_.reserve(static_cast<std::size_t>(capacity_));
	}
	StringBuilder::StringBuilder(const jxx::Ptr<CharSequence>& sequence) : value_(), capacity_(0)
	{
		if (sequence == nullptr) throwNPE_();
		value_ = toUtf16_(sequence);
		capacity_ = static_cast<jint>(value_.size()) + DEFAULT_CAPACITY;
		value_.reserve(static_cast<std::size_t>(capacity_));
	}

	jint StringBuilder::length() const
	{
		return static_cast<jint>(value_.size());
	}
	jint StringBuilder::capacity() const
	{
		return capacity_;
	}
	jint StringBuilder::newCapacity_(jint minimum) const
	{
		std::int64_t candidate = static_cast<std::int64_t>(capacity_) * 2 + 2;
		if (candidate < minimum) candidate = minimum;
		if (candidate > std::numeric_limits<jint>::max()) {
			if (minimum < 0) throw OutOfMemoryError();
			candidate = std::numeric_limits<jint>::max();
		}
		return static_cast<jint>(candidate);
	}
	void StringBuilder::ensureCapacityInternal_(jint minimum)
	{
		if (minimum <= capacity_) return;
		capacity_ = newCapacity_(minimum);
		value_.reserve(static_cast<std::size_t>(capacity_));
	}
	void StringBuilder::ensureCapacity(jint minimum)
	{
		if (minimum > 0) ensureCapacityInternal_(minimum);
	}
	void StringBuilder::trimToSize()
	{
		capacity_ = length(); std::u16string compact(value_); value_.swap(compact);
	}
	void StringBuilder::setLength(jint newLength)
	{
		if (newLength < 0) throwSIOOBE_();
		ensureCapacityInternal_(newLength);
		value_.resize(static_cast<std::size_t>(newLength), u'\0');
	}

	jchar StringBuilder::charAt(jint index) const
	{
		if (index < 0 || index >= length()) throwSIOOBE_(); return static_cast<jchar>(value_[index]);
	}
	void StringBuilder::setCharAt(jint index, jchar value)
	{
		if (index < 0 || index >= length()) throwSIOOBE_(); value_[index] = static_cast<char16_t>(value);
	}

	jint StringBuilder::codePointAt(jint index) const
	{
		if (index < 0 || index >= length()) throwSIOOBE_();
		const auto first = value_[index];
		if (isHigh_(first) && index + 1 < length() && isLow_(value_[index + 1])) return ((first - 0xD800) << 10) + (value_[index + 1] - 0xDC00) + 0x10000;
		return first;
	}
	jint StringBuilder::codePointBefore(jint index) const
	{
		if (index <= 0 || index > length()) throwSIOOBE_();
		const auto second = value_[index - 1];
		if (isLow_(second) && index > 1 && isHigh_(value_[index - 2])) return ((value_[index - 2] - 0xD800) << 10) + (second - 0xDC00) + 0x10000;
		return second;
	}
	jint StringBuilder::codePointCount(jint begin, jint end) const
	{
		if (begin < 0 || end<begin || end>length()) throwSIOOBE_();
		jint count = 0; for (jint i = begin; i < end; ++i, ++count) if (isHigh_(value_[i]) && i + 1 < end && isLow_(value_[i + 1])) ++i; return count;
	}
	jint StringBuilder::offsetByCodePoints(jint index, jint offset) const
	{
		if (index<0 || index>length()) throwSIOOBE_(); jint i = index;
		if (offset >= 0) {
			for (jint n = 0; n < offset; ++n) {
				if (i >= length())throwSIOOBE_(); auto c = value_[i++]; if (isHigh_(c) && i < length() && isLow_(value_[i]))++i;
			}
		}
		else {
			for (jint n = 0; n < -offset; ++n) {
				if (i <= 0)throwSIOOBE_(); auto c = value_[--i]; if (isLow_(c) && i > 0 && isHigh_(value_[i - 1]))--i;
			}
		} return i;
	}

	std::u16string StringBuilder::toUtf16_(const jxx::Ptr<CharSequence>& sequence)
	{
		if (sequence == nullptr) throwNPE_(); std::u16string result; result.reserve(sequence->length());
		for (jint i = 0; i < sequence->length(); ++i) result.push_back(static_cast<char16_t>(sequence->charAt(i))); return result;
	}
	void StringBuilder::appendUtf16_(const std::u16string& text)
	{
		if (text.empty())return; const auto required = value_.size() + text.size();
		if (required > static_cast<std::size_t>(std::numeric_limits<jint>::max())) throw jxx::lang::OutOfMemoryError();
		ensureCapacityInternal_(static_cast<jint>(required)); value_.append(text);
	}
	void StringBuilder::insertUtf16_(jint offset, const std::u16string& text)
	{
		if (offset<0 || offset>length())throwSIOOBE_(); if (text.empty())return;
		const auto required = value_.size() + text.size(); if (required > static_cast<std::size_t>(std::numeric_limits<jint>::max()))throw OutOfMemoryError();
		ensureCapacityInternal_(static_cast<jint>(required)); value_.insert(static_cast<std::size_t>(offset), text);
	}

	jxx::Ptr<StringBuilder> StringBuilder::append(jbool v)
	{
		return append(jxx::NEW<String>(v ? "true" : "false"));
	}
	jxx::Ptr<Appendable> StringBuilder::append(jchar v)
	{
		ensureCapacityInternal_(length() + 1); value_.push_back(v); return jxx::CAST<Appendable>(self_());
	}
	jxx::Ptr<StringBuilder> StringBuilder::appendSB(jchar v)
	{
		ensureCapacityInternal_(length() + 1); value_.push_back(v); return self_();
	}
	jxx::Ptr<StringBuilder> StringBuilder::append(const CharArray& a)
	{
		return a == nullptr ? append(jxx::NEW<String>("null")) : append(a, 0, a->length);
	}
	jxx::Ptr<StringBuilder> StringBuilder::append(const CharArray& a, jint o, jint n)
	{
		if (a == nullptr)return append(jxx::NEW<String>("null")); if (o < 0 || n<0 || o + n>a->length)throwSIOOBE_(); appendUtf16_(charArrayText(a, o, n)); return self_();
	}
	jxx::Ptr<Appendable> StringBuilder::append(const jxx::Ptr<CharSequence> s)
	{
		appendUtf16_(s == nullptr ? u"null" : toUtf16_(s)); return jxx::CAST<Appendable>(self_());
	}
	jxx::Ptr<StringBuilder> StringBuilder::appendSB(const jxx::Ptr<CharSequence> s)
	{
		appendUtf16_(s == nullptr ? u"null" : toUtf16_(s)); return self_();
	}
	jxx::Ptr<Appendable> StringBuilder::append(const jxx::Ptr<CharSequence> s, jint start, jint end)
	{
		auto text = s == nullptr ? std::u16string(u"null") : toUtf16_(s); if (start < 0 || end<start || end>static_cast<jint>(text.size()))throwSIOOBE_(); appendUtf16_(text.substr(start, end - start)); return jxx::CAST<Appendable>(self_());
	}
	jxx::Ptr<StringBuilder> StringBuilder::appendSB(const jxx::Ptr<CharSequence> s, jint start, jint end)
	{
		append(s, start, end); return self_();
	}
	jxx::Ptr<StringBuilder> StringBuilder::append(jdouble v)
	{
		std::ostringstream o; o.imbue(std::locale::classic()); o << v; return append(jxx::NEW<String>(o.str()));
	}
	jxx::Ptr<StringBuilder> StringBuilder::append(jfloat v)
	{
		std::ostringstream o; o.imbue(std::locale::classic()); o << v; return append(jxx::NEW<String>(o.str()));
	}
	jxx::Ptr<StringBuilder> StringBuilder::append(jint v)
	{
		return append(jxx::NEW<String>(std::to_string(v)));
	}
	jxx::Ptr<StringBuilder> StringBuilder::append(jlong v)
	{
		return append(jxx::NEW<String>(std::to_string(v)));
	}
	jxx::Ptr<StringBuilder> StringBuilder::append(const jxx::Ptr<Object> v)
	{
		return append(v == nullptr ? jxx::NEW<String>("null") : v->toString());
	}
	jxx::Ptr<StringBuilder> StringBuilder::append(const jxx::Ptr<String> v)
	{
		appendUtf16_(v == nullptr ? u"null" : v->utf16()); return self_();
	}
	jxx::Ptr<StringBuilder> StringBuilder::append(const jxx::Ptr<StringBuffer> v)
	{
		return append(v == nullptr ? jxx::NEW<String>("null") : v->toString());
	}
	jxx::Ptr<StringBuilder> StringBuilder::appendCodePoint(jint cp)
	{
		if (cp < 0 || cp>0x10FFFF)throwIAE_("Invalid code point"); ensureCapacityInternal_(length() + (cp <= 0xFFFF ? 1 : 2)); if (cp <= 0xFFFF)value_.push_back(cp); else {
			cp -= 0x10000; value_.push_back(0xD800 + (cp >> 10)); value_.push_back(0xDC00 + (cp & 0x3FF));
		}return self_();
	}

	jxx::Ptr<StringBuilder> StringBuilder::delete_(jint s, jint e)
	{
		if (s < 0)throwSIOOBE_(); e = std::min(e, length()); if (s > e)throwSIOOBE_(); value_.erase(s, e - s); return self_();
	}
	jxx::Ptr<StringBuilder> StringBuilder::deleteCharAt(jint i)
	{
		if (i < 0 || i >= length())throwSIOOBE_(); value_.erase(i, 1); return self_();
	}
	jxx::Ptr<StringBuilder> StringBuilder::replace(jint s, jint e, const jxx::Ptr<String>& v)
	{
		if (s<0 || s>e || s > length())throwSIOOBE_(); e = std::min(e, length()); auto t = v == nullptr ? std::u16string(u"null") : v->utf16(); value_.erase(s, e - s); insertUtf16_(s, t); return self_();
	}
	jxx::Ptr<StringBuilder> StringBuilder::reverse()
	{
		std::reverse(value_.begin(), value_.end()); for (std::size_t i = 0; i + 1 < value_.size(); ++i)if (isLow_(value_[i]) && isHigh_(value_[i + 1])) {
			std::swap(value_[i], value_[i + 1]); ++i;
		}return self_();
	}

	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, jbool v)
	{
		return insert(o, jxx::NEW<String>(v ? "true" : "false"));
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, jchar v)
	{
		if (o<0 || o>length())throwSIOOBE_(); ensureCapacityInternal_(length() + 1); value_.insert(value_.begin() + o, v); return self_();
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, const CharArray a)
	{
		return a == nullptr ? insert(o, jxx::NEW<String>("null")) : insert(o, a, 0, a->length);
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint i, const CharArray a, jint o, jint n)
	{
		if (a == nullptr)return insert(i, jxx::NEW<String>("null")); if (o < 0 || n<0 || o + n>a->length)throwSIOOBE_(); insertUtf16_(i, charArrayText(a, o, n)); return self_();
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, const jxx::Ptr<CharSequence> s)
	{
		insertUtf16_(o, s == nullptr ? u"null" : toUtf16_(s)); return self_();
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, const jxx::Ptr<CharSequence> s, jint start, jint end)
	{
		auto t = s == nullptr ? std::u16string(u"null") : toUtf16_(s); if (start < 0 || end<start || end>static_cast<jint>(t.size()))throwSIOOBE_(); insertUtf16_(o, t.substr(start, end - start)); return self_();
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, jdouble v)
	{
		std::ostringstream s; s.imbue(std::locale::classic()); s << v; return insert(o, jxx::NEW<String>(s.str()));
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, jfloat v)
	{
		std::ostringstream s; s.imbue(std::locale::classic()); s << v; return insert(o, jxx::NEW<String>(s.str()));
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, jint v)
	{
		return insert(o, jxx::NEW<String>(std::to_string(v)));
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, jlong v)
	{
		return insert(o, jxx::NEW<String>(std::to_string(v)));
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, const jxx::Ptr<Object> v)
	{
		return insert(o, v == nullptr ? jxx::NEW<String>("null") : v->toString());
	}
	jxx::Ptr<StringBuilder> StringBuilder::insert(jint o, const jxx::Ptr<String> v)
	{
		insertUtf16_(o, v == nullptr ? u"null" : v->utf16()); return self_();
	}

	void StringBuilder::getChars(jint b, jint e, const CharArray& d, jint db) const
	{
		if (d == nullptr)throwNPE_(); if (b < 0 || e<b || e>length() || db<0 || db + e - b>d->length)throwSIOOBE_(); for (jint i = 0; i < e - b; ++i)(*d)[db + i] = value_[b + i];
	}
	jint StringBuilder::indexOf(const jxx::Ptr<String>& s) const
	{
		return indexOf(s, 0);
	}
	jint StringBuilder::indexOf(const jxx::Ptr<String>& s, jint from) const
	{
		if (s == nullptr)throwNPE_(); from = std::max(0, from); if (from > length())return -1; auto p = value_.find(s->utf16(), from); return p == std::u16string::npos ? -1 : static_cast<jint>(p);
	}
	jint StringBuilder::lastIndexOf(const jxx::Ptr<String>& s) const
	{
		return lastIndexOf(s, length());
	}
	jint StringBuilder::lastIndexOf(const jxx::Ptr<String>& s, jint from) const
	{
		if (s == nullptr)throwNPE_(); if (from < 0)return -1; from = std::min(from, length()); auto p = value_.rfind(s->utf16(), from); return p == std::u16string::npos ? -1 : static_cast<jint>(p);
	}
	jxx::Ptr<String> StringBuilder::substring(jint s) const
	{
		return substring(s, length());
	}
	jxx::Ptr<String> StringBuilder::substring(jint s, jint e) const
	{
		if (s < 0 || e<s || e>length())throwSIOOBE_(); return jxx::NEW<String>(value_.substr(s, e - s));
	}
	jxx::Ptr<CharSequence> StringBuilder::subSequence(jint s, jint e) const
	{
		return jxx::CAST<CharSequence>(substring(s, e));
	}
	jxx::Ptr<String> StringBuilder::toString() const
	{
		return jxx::NEW<String>(value_);
	}

	void StringBuilder::writeObject(const jxx::Ptr<jxx::io::ObjectOutputStream>& out)
	{
		if (out == nullptr)throwNPE_(); out->writeInt(length()); for (auto c : value_)out->writeChar(c);
	}
	void StringBuilder::readObject(const jxx::Ptr<jxx::io::ObjectInputStream>& in)
	{
		if (in == nullptr)throwNPE_(); const auto n = in->readInt(); if (n < 0)throw IllegalArgumentException(); value_.clear(); capacity_ = n + DEFAULT_CAPACITY; value_.reserve(capacity_); for (jint i = 0; i < n; ++i)value_.push_back(in->readChar());
	}
	void StringBuilder::readObjectNoData()
	{
		value_.clear(); capacity_ = DEFAULT_CAPACITY; value_.reserve(capacity_);
	}
	jxx::Ptr<Object> StringBuilder::cloneImpl() const
	{
		throw CloneNotSupportedException();
	}

} // namespace jxx::lang
