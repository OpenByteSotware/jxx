#include <string>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <algorithm>
#include <cctype>
#include "lang/jxx.lang.String.h"
#include "math/jxx.math.BigInteger.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

namespace jxx::math
{
	static std::string narrowAscii_(const jxx::Ptr<jxx::lang::String>& s)
	{
		if (!s) {
			throw std::runtime_error("null String");
		}

		const std::u16string u = s->utf16();
		std::string out;
		out.reserve(u.size());

		for (char16_t ch : u) {
			if (ch > 0x7f) {
				throw std::runtime_error("non-ascii BigInteger string not supported");
			}
			out.push_back(static_cast<char>(ch));
		}

		return out;
	}

	static jxx::Ptr<jxx::lang::String> stringFromAscii_(const std::string& s)
	{
		return jxx::NEW<jxx::lang::String>(s.c_str());
	}

	static bool isAllDigits_(const std::string& s)
	{
		for (char c : s) {
			if (c < '0' || c > '9') {
				return false;
			}
		}
		return !s.empty();
	}

	static std::string stripLeadingZeros_(const std::string& s)
	{
		std::size_t i = 0;
		while (i + 1 < s.size() && s[i] == '0') {
			++i;
		}
		return s.substr(i);
	}


	jxx::Ptr<BigInteger> BigInteger::ZERO = jxx::NEW<BigInteger>(static_cast<jxx::lang::jlong>(0));
	jxx::Ptr<BigInteger> BigInteger::ONE = jxx::NEW<BigInteger>(static_cast<jxx::lang::jlong>(1));
	jxx::Ptr<BigInteger> BigInteger::TEN = jxx::NEW<BigInteger>(static_cast<jxx::lang::jlong>(10));

	BigInteger::BigInteger(jxx::lang::jlong value)
		: negative_(value < 0),
		digits_()
	{
		if (value == 0) {
			digits_ = "0";
			negative_ = false;
			return;
		}

		unsigned long long mag =
			negative_
			? static_cast<unsigned long long>(-(value + 1)) + 1ULL
			: static_cast<unsigned long long>(value);

		digits_ = std::to_string(mag);
		normalize_(negative_, digits_);
	}

	BigInteger::BigInteger(const jxx::Ptr<jxx::lang::String>& value)
		: negative_(false),
		digits_(parseDigits_(value, negative_))
	{
		normalize_(negative_, digits_);
	}

	BigInteger::BigInteger(
		const jxx::Ptr<jxx::lang::String>& value,
		jxx::lang::jint radix)
		: negative_(false), digits_("0")
	{
		if (value == nullptr) throw jxx::lang::NullPointerException();
		if (radix < 2 || radix > 36) throw std::runtime_error("invalid radix");
		const std::string text = narrowAscii_(value);
		std::size_t index = 0;
		if (!text.empty() && (text[0] == '+' || text[0] == '-')) {
			negative_ = text[0] == '-'; ++index;
		}
		if (index == text.size()) throw std::runtime_error("invalid BigInteger");
		digits_ = "0";
		for (; index < text.size(); ++index) {
			int digit = -1;
			const char ch = text[index];
			if (ch >= '0' && ch <= '9') digit = ch - '0';
			else if (ch >= 'a' && ch <= 'z') digit = ch - 'a' + 10;
			else if (ch >= 'A' && ch <= 'Z') digit = ch - 'A' + 10;
			if (digit < 0 || digit >= radix) throw std::runtime_error("invalid BigInteger");
			int carry = digit;
			for (auto i = digits_.size(); i > 0; --i) {
				const int v = (digits_[i - 1] - '0') * radix + carry;
				digits_[i - 1] = static_cast<char>('0' + v % 10);
				carry = v / 10;
			}
			while (carry) {
				digits_.insert(digits_.begin(), static_cast<char>('0' + carry % 10)); carry /= 10;
			}
		}
		normalize_(negative_, digits_);
	}

	jxx::Ptr<BigInteger> BigInteger::valueOf(jxx::lang::jlong value)
	{
		return jxx::NEW<BigInteger>(value);
	}

	jxx::Ptr<BigInteger> BigInteger::parse(const jxx::Ptr<jxx::lang::String>& value)
	{
		return jxx::NEW<BigInteger>(value);
	}


	jxx::lang::jint BigInteger::compareTo(const jxx::Ptr<BigInteger>& other) const
	{
		if (other == nullptr) {
			throw jxx::lang::NullPointerException();
		}
		if (negative_ != other->negative_) {
			return negative_ ? -1 : 1;
		}
		jxx::lang::jint result = 0;
		if (digits_.size() != other->digits_.size()) {
			result = digits_.size() < other->digits_.size() ? -1 : 1;
		}
		else if (digits_ != other->digits_) {
			result = digits_ < other->digits_ ? -1 : 1;
		}
		return negative_ ? -result : result;
	}

	jxx::lang::jint BigInteger::signum() const
	{
		if (digits_ == "0") {
			return 0;
		}
		return negative_ ? -1 : 1;
	}

	jxx::Ptr<BigInteger> BigInteger::abs() const
	{
		return negative_ ? jxx::NEW<BigInteger>(stringFromAscii_(digits_))
			: jxx::NEW<BigInteger>(this->toString());
	}

	jxx::Ptr<BigInteger> BigInteger::negate() const
	{
		if (digits_ == "0") {
			return jxx::NEW<BigInteger>(0LL);
		}

		std::string s = negative_ ? digits_ : ("-" + digits_);
		return jxx::NEW<BigInteger>(stringFromAscii_(s));
	}

	namespace
	{
		int cmpMag(const std::string& a, const std::string& b)
		{
			if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
			return a == b ? 0 : (a < b ? -1 : 1);
		}
		std::string addMag(const std::string& a, const std::string& b)
		{
			std::string r; int carry = 0; std::size_t i = a.size(), j = b.size();
			while (i || j || carry) {
				int v = carry; if (i)v += a[--i] - '0'; if (j)v += b[--j] - '0'; r.push_back(char('0' + v % 10)); carry = v / 10;
			}
			std::reverse(r.begin(), r.end()); return r;
		}
		std::string subMag(const std::string& a, const std::string& b)
		{
			std::string r; int borrow = 0; std::size_t i = a.size(), j = b.size();
			while (i) {
				int v = (a[--i] - '0') - borrow - (j ? b[--j] - '0' : 0); if (v < 0) {
					v += 10; borrow = 1;
				}
				else borrow = 0; r.push_back(char('0' + v));
			}
			while (r.size() > 1 && r.back() == '0')r.pop_back(); std::reverse(r.begin(), r.end()); return r;
		}
		std::string mulMag(const std::string& a, const std::string& b)
		{
			std::vector<int> v(a.size() + b.size()); for (std::size_t i = a.size(); i--;)for (std::size_t j = b.size(); j--;)v[i + j + 1] += (a[i] - '0') * (b[j] - '0');
			for (std::size_t i = v.size(); i-- > 1;) {
				v[i - 1] += v[i] / 10; v[i] %= 10;
			}std::string r; std::size_t i = 0; while (i + 1 < v.size() && v[i] == 0)++i; for (; i < v.size(); ++i)r.push_back(char('0' + v[i])); return r;
		}
		void divMag(const std::string& a, const std::string& b, std::string& q, std::string& rem)
		{
			if (b == "0")throw std::runtime_error("division by zero"); q.clear(); rem = "0";
			for (char c : a) {
				rem = rem == "0" ? std::string(1, c) : rem + c; while (rem.size() > 1 && rem[0] == '0')rem.erase(rem.begin()); int d = 0; while (cmpMag(rem, b) >= 0) {
					rem = subMag(rem, b); ++d;
				}q.push_back(char('0' + d));
			}
			auto p = q.find_first_not_of('0'); q = p == std::string::npos ? "0" : q.substr(p);
		}
	}
	jxx::Ptr<BigInteger> BigInteger::add(const jxx::Ptr<BigInteger>& v) const
	{
		if (!v)throw jxx::lang::NullPointerException(); bool n; std::string d; if (negative_ == v->negative_) {
			n = negative_; d = addMag(digits_, v->digits_);
		}
		else {
			int c = cmpMag(digits_, v->digits_); n = c >= 0 ? negative_ : v->negative_; d = c >= 0 ? subMag(digits_, v->digits_) : subMag(v->digits_, digits_);
		}return jxx::NEW<BigInteger>(stringFromAscii_((n && d != "0" ? "-" : "") + d));
	}
	jxx::Ptr<BigInteger> BigInteger::subtract(const jxx::Ptr<BigInteger>& v) const
	{
		if (!v)throw jxx::lang::NullPointerException(); return add(v->negate());
	}
	jxx::Ptr<BigInteger> BigInteger::multiply(const jxx::Ptr<BigInteger>& v) const
	{
		if (!v)throw jxx::lang::NullPointerException(); auto d = mulMag(digits_, v->digits_); return jxx::NEW<BigInteger>(stringFromAscii_(((negative_ != v->negative_) && d != "0" ? "-" : "") + d));
	}
	jxx::Ptr<::jxx::JxxArray<jxx::Ptr<BigInteger>, 1U>> BigInteger::divideAndRemainder(const jxx::Ptr<BigInteger>& v) const
	{
		if (!v) throw jxx::lang::NullPointerException();
		std::string q;
		std::string r;
		divMag(digits_, v->digits_, q, r);
		const auto quotient = jxx::NEW<BigInteger>(stringFromAscii_(
			((negative_ != v->negative_) && q != "0" ? "-" : "") + q));
		const auto remainderValue = jxx::NEW<BigInteger>(stringFromAscii_(
			(negative_ && r != "0" ? "-" : "") + r));
		const auto result = jxx::NEW<
			::jxx::JxxArray<jxx::Ptr<BigInteger>, 1U>>(
				static_cast<jxx::lang::jint>(2));
		(*result)(0) = quotient;
		(*result)(1) = remainderValue;
		return result;
	}
	jxx::Ptr<BigInteger> BigInteger::divide(const jxx::Ptr<BigInteger>& v) const
	{
		const auto result = divideAndRemainder(v);
		return (*result)(0);
	}
	jxx::Ptr<BigInteger> BigInteger::remainder(const jxx::Ptr<BigInteger>& v) const
	{
		const auto result = divideAndRemainder(v);
		return (*result)(1);
	}
	jxx::Ptr<BigInteger> BigInteger::pow(jxx::lang::jint e) const
	{
		if (e < 0)throw std::runtime_error("negative exponent"); auto base = jxx::NEW<BigInteger>(stringFromAscii_((negative_ ? "-" : "") + digits_)); auto r = ONE; while (e) {
			if (e & 1)r = r->multiply(base); e >>= 1; if (e)base = base->multiply(base);
		}return r;
	}
	jxx::Ptr<BigInteger> BigInteger::gcd(const jxx::Ptr<BigInteger>& v) const
	{
		if (!v)throw jxx::lang::NullPointerException(); auto a = abs(), b = v->abs(); while (b->signum() != 0) {
			auto r = a->remainder(b); a = b; b = r;
		}return a;
	}
	jxx::Ptr<BigInteger> BigInteger::min(const jxx::Ptr<BigInteger>& v) const
	{
		if (!v)throw jxx::lang::NullPointerException(); return compareTo(v) <= 0 ? jxx::NEW<BigInteger>(stringFromAscii_((negative_ ? "-" : "") + digits_)) : v;
	}
	jxx::Ptr<BigInteger> BigInteger::max(const jxx::Ptr<BigInteger>& v) const
	{
		if (!v)throw jxx::lang::NullPointerException(); return compareTo(v) >= 0 ? jxx::NEW<BigInteger>(stringFromAscii_((negative_ ? "-" : "") + digits_)) : v;
	}

	jxx::Ptr<jxx::lang::String> BigInteger::toString() const
	{
		return stringFromAscii_(negative_ && digits_ != "0" ? "-" + digits_ : digits_);
	}

	jxx::Ptr<jxx::lang::String> BigInteger::toString(jxx::lang::jint radix) const
	{
		if (radix < 2 || radix > 36) {
			throw std::runtime_error("radix out of range");
		}

		if (digits_ == "0") {
			return stringFromAscii_("0");
		}

		const char* alphabet = "0123456789abcdefghijklmnopqrstuvwxyz";
		std::string value = digits_;
		std::string out;

		while (value != "0") {
			jxx::lang::jint rem = 0;
			value = divideBySmallBase_(value, radix, rem);
			out.push_back(alphabet[rem]);
		}

		if (negative_) {
			out.push_back('-');
		}

		std::reverse(out.begin(), out.end());
		return stringFromAscii_(out);
	}

	jxx::lang::jbyte BigInteger::byteValue() const
	{
		return static_cast<jxx::lang::jbyte>(intValue());
	}

	jxx::lang::jshort BigInteger::shortValue() const
	{
		return static_cast<jxx::lang::jshort>(intValue());
	}

	jxx::lang::jint BigInteger::intValue() const
	{
		return static_cast<jxx::lang::jint>(longValue());
	}

	jxx::lang::jlong BigInteger::longValue() const
	{
		std::string s = negative_ ? "-" + digits_ : digits_;
		return static_cast<jxx::lang::jlong>(std::stoll(s));
	}

	jxx::lang::jfloat BigInteger::floatValue() const
	{
		std::string s = negative_ ? "-" + digits_ : digits_;
		return static_cast<jxx::lang::jfloat>(std::stof(s));
	}

	jxx::lang::jdouble BigInteger::doubleValue() const
	{
		std::string s = negative_ ? "-" + digits_ : digits_;
		return static_cast<jxx::lang::jdouble>(std::stod(s));
	}

	jxx::lang::jint BigInteger::hashCode() const
	{
		jxx::lang::jint h = 1;
		for (char c : digits_) {
			h = 31 * h + static_cast<jxx::lang::jint>(c);
		}
		return negative_ ? -h : h;
	}

	jxx::lang::jbool BigInteger::equals(const jxx::Ptr<Object>& obj) const
	{
		jxx::Ptr<BigInteger> other = jxx::CAST<BigInteger, Object>(obj);
		return other ? (other->negative_ == negative_ && other->digits_ == digits_) : false;
	}

	void BigInteger::normalize_(jxx::lang::jbool& negative, std::string& digits)
	{
		if (digits.empty()) {
			digits = "0";
			negative = false;
			return;
		}

		digits = stripLeadingZeros_(digits);

		if (digits == "0") {
			negative = false;
		}
	}

	std::string BigInteger::parseDigits_(const jxx::Ptr<jxx::lang::String>& value, jxx::lang::jbool& negative)
	{
		std::string s = narrowAscii_(value);
		if (s.empty()) {
			throw std::runtime_error("empty BigInteger");
		}

		std::size_t i = 0;
		negative = false;

		if (s[i] == '+' || s[i] == '-') {
			negative = (s[i] == '-');
			++i;
		}

		if (i >= s.size()) {
			throw std::runtime_error("invalid BigInteger");
		}

		std::string digits = s.substr(i);
		if (!isAllDigits_(digits)) {
			throw std::runtime_error("invalid BigInteger digits");
		}

		return stripLeadingZeros_(digits);
	}

	std::string BigInteger::divideBySmallBase_(const std::string& dec, jxx::lang::jint base, jxx::lang::jint& remainder)
	{
		std::string q;
		q.reserve(dec.size());

		jxx::lang::jint carry = 0;
		for (char c : dec) {
			jxx::lang::jint cur = carry * 10 + (c - '0');
			jxx::lang::jint digit = cur / base;
			carry = cur % base;

			if (!q.empty() || digit != 0) {
				q.push_back(static_cast<char>('0' + digit));
			}
		}

		if (q.empty()) {
			q = "0";
		}

		remainder = carry;
		return q;
	}

	void BigInteger::writeObject(
		const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out)
	{
		if (out == nullptr) throw ::jxx::lang::NullPointerException();
		out->writeBoolean(negative_);
		out->writeObject(::jxx::CAST<::jxx::lang::Object>(
			::jxx::NEW<::jxx::lang::String>(digits_)));
	}

	void BigInteger::readObject(
		const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in)
	{
		if (in == nullptr) throw ::jxx::lang::NullPointerException();
		negative_ = in->readBoolean();
		digits_ = narrowAscii_(::jxx::CAST<::jxx::lang::String>(
			in->readObject()));
		normalize_(negative_, digits_);
	}

	void BigInteger::readObjectNoData()
	{
		negative_ = false;
		digits_ = "0";
	}

}