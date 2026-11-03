#include "awt/jxx.awt.Font.h"
#include <cmath>
#include <functional>
#include <sstream>
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "io/jxx.io.InvalidObjectException.h"
#include "lang/jxx.lang.NullPointerException.h"
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


    void Font::writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output)
    {
        if (output == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        output->writeObject(name_);
        output->writeInt(style_);
        output->writeFloat(size_);
    }

    void Font::readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input)
    {
        if (input == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        const auto name = ::jxx::CAST<::jxx::lang::String>(input->readObject());
        const auto style = input->readInt();
        const auto size = input->readFloat();
        if (name == nullptr) {
            throw ::jxx::io::InvalidObjectException(
                ::jxx::NEW<::jxx::lang::String>("Font name is null"));
        }
        try {
            validStyle(style);
        } catch (const ::jxx::lang::IllegalArgumentException&) {
            throw ::jxx::io::InvalidObjectException(
                ::jxx::NEW<::jxx::lang::String>("Invalid font style"));
        }
        if (!std::isfinite(size)) {
            throw ::jxx::io::InvalidObjectException(
                ::jxx::NEW<::jxx::lang::String>("Invalid font size"));
        }
        name_ = name;
        style_ = style;
        size_ = size;
    }

    void Font::readObjectNoData()
    {
        name_ = ::jxx::NEW<::jxx::lang::String>("Default");
        style_ = PLAIN;
        size_ = 12.0F;
    }
}
