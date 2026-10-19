#include "awt/jxx.awt.Font.h"
#include <cmath>
#include <functional>
#include <sstream>
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::awt
{		void validStyle(::jxx::lang::jint s)
		{
			if ((s & ~3) != 0)throw ::jxx::lang::IllegalArgumentException("Invalid font style");
		}
	
	Font::Font(const ::jxx::Ptr<::jxx::lang::String>& n, ::jxx::lang::jint s, ::jxx::lang::jint z) :
		name_(n ? n : jxx::NEW<::jxx::lang::String>("Default")), style_(s), size_(static_cast<::jxx::lang::jfloat>(z))
	{
		validStyle(s);
	}
	::jxx::Ptr<::jxx::lang::String> Font::getName() const
	{
		return name_;
	} ::jxx::Ptr<::jxx::lang::String> Font::getFamily() const
	{
		return name_;
	} ::jxx::lang::jint Font::getStyle() const
	{
		return style_;
	} ::jxx::lang::jint Font::getSize() const
	{
		return static_cast<::jxx::lang::jint>(size_);
	} ::jxx::lang::jfloat Font::getSize2D() const
	{
		return size_;
	} ::jxx::lang::jbool Font::isPlain() const
	{
		return style_ == PLAIN;
	} ::jxx::lang::jbool Font::isBold() const
	{
		return (style_ & BOLD) != 0;
	} ::jxx::lang::jbool Font::isItalic() const
	{
		return (style_ & ITALIC) != 0;
	}
	::jxx::Ptr<Font> Font::deriveFont(::jxx::lang::jint s) const
	{
		validStyle(s); return ::jxx::NEW<Font>(name_, s, getSize());
	} ::jxx::Ptr<Font> Font::deriveFont(::jxx::lang::jfloat z) const
	{
		return deriveFont(style_, z);
	} ::jxx::Ptr<Font> Font::deriveFont(::jxx::lang::jint s, ::jxx::lang::jfloat z) const
	{
		validStyle(s); auto f = ::jxx::NEW<Font>(name_, s, static_cast<::jxx::lang::jint>(z)); f->size_ = z; return f;
	}
	::jxx::lang::jbool Font::equals(const ::jxx::Ptr<::jxx::lang::Object>& o) const
	{
		auto f = ::jxx::CAST<Font>(o); return f && name_->equals(f->name_) && f->style_ == style_ && f->size_ == size_;
	} ::jxx::lang::jint Font::hashCode() const
	{
		return name_->hashCode() * 31 + style_ * 7 + static_cast<::jxx::lang::jint>(size_);
	} ::jxx::Ptr<::jxx::lang::String> Font::toString() const
	{
		std::ostringstream s; s << "jxx.awt.Font[family=" << name_->utf8() << ",style=" << style_ << ",size=" << getSize() << "]"; return ::jxx::NEW<::jxx::lang::String>(s.str());
	}
}
