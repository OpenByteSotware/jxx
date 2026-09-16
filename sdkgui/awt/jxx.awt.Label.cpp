#include "awt/jxx.awt.Label.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::awt {
namespace { ::jxx::Ptr<::jxx::lang::String> emptyText(){return ::jxx::NEW<::jxx::lang::String>("");} }
Label::Label():Label(emptyText(),LEFT){} Label::Label(const ::jxx::Ptr<::jxx::lang::String>& text):Label(text,LEFT){}
Label::Label(const ::jxx::Ptr<::jxx::lang::String>& text,::jxx::lang::jint alignment):text_(text ? text : emptyText()),alignment_(LEFT){setAlignment(alignment);}
::jxx::Ptr<::jxx::lang::String> Label::getText() const{return text_;} void Label::setText(const ::jxx::Ptr<::jxx::lang::String>& text){text_=text ? text : emptyText();invalidate();}
::jxx::lang::jint Label::getAlignment() const{return alignment_;} void Label::setAlignment(::jxx::lang::jint value){if(value<LEFT||value>RIGHT)throw ::jxx::lang::IllegalArgumentException("alignment");alignment_=value;invalidate();}
}
