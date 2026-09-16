#include "awt/jxx.awt.TextComponent.h"
#include <algorithm>
#include "gui/internal/jxx.gui.internal.NativeComponent.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::awt {
namespace { ::jxx::Ptr<::jxx::lang::String> emptyText(){return ::jxx::NEW<::jxx::lang::String>("");} }
TextComponent::TextComponent(const ::jxx::Ptr<::jxx::lang::String>& value):text_(value ? value : emptyText()),editable_(true),selectionStart_(0),selectionEnd_(0),caretPosition_(0){} TextComponent::~TextComponent()=default;
::jxx::Ptr<::jxx::lang::String> TextComponent::getText() const{return nativeComponent_ ? nativeComponent_->getText() : text_;} void TextComponent::setText(const ::jxx::Ptr<::jxx::lang::String>& value){text_=value ? value : emptyText();if(nativeComponent_)nativeComponent_->setText(text_);selectionStart_=selectionEnd_=caretPosition_=0;invalidate();}
::jxx::lang::jbool TextComponent::isEditable() const{return editable_;} void TextComponent::setEditable(::jxx::lang::jbool value){editable_=value;if(nativeComponent_)nativeComponent_->setEditable(value);}
::jxx::lang::jint TextComponent::getSelectionStart() const{return selectionStart_;} void TextComponent::setSelectionStart(::jxx::lang::jint value){select(value,selectionEnd_);} ::jxx::lang::jint TextComponent::getSelectionEnd() const{return selectionEnd_;} void TextComponent::setSelectionEnd(::jxx::lang::jint value){select(selectionStart_,value);}
void TextComponent::select(::jxx::lang::jint start,::jxx::lang::jint end){if(start<0||end<0)throw ::jxx::lang::IllegalArgumentException("selection");selectionStart_=std::min(start,end);selectionEnd_=std::max(start,end);caretPosition_=selectionEnd_;if(nativeComponent_)nativeComponent_->setSelection(selectionStart_,selectionEnd_);} void TextComponent::selectAll(){selectionStart_=0;selectionEnd_=static_cast<::jxx::lang::jint>(getText()->utf8().size());caretPosition_=selectionEnd_;if(nativeComponent_)nativeComponent_->setSelection(selectionStart_,selectionEnd_);}
::jxx::Ptr<::jxx::lang::String> TextComponent::getSelectedText() const{const auto value=getText()->utf8();const auto start=std::min<std::size_t>(selectionStart_,value.size());const auto end=std::min<std::size_t>(selectionEnd_,value.size());return ::jxx::NEW<::jxx::lang::String>(value.substr(start,end-start));}
::jxx::lang::jint TextComponent::getCaretPosition() const{return caretPosition_;} void TextComponent::setCaretPosition(::jxx::lang::jint value){if(value<0)throw ::jxx::lang::IllegalArgumentException("position");caretPosition_=value;selectionStart_=selectionEnd_=value;if(nativeComponent_)nativeComponent_->setCaretPosition(value);}
}
