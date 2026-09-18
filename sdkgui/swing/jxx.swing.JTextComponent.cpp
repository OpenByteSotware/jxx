#include "swing/jxx.swing.JTextComponent.h"
#include <algorithm>
#include "gui/internal/jxx.gui.internal.NativeComponent.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::swing
{
    namespace { ::jxx::Ptr<::jxx::lang::String> emptyText(){ return ::jxx::NEW<::jxx::lang::String>(""); } }
    JTextComponent::JTextComponent() : JTextComponent(emptyText()) {}
    JTextComponent::JTextComponent(const ::jxx::Ptr<::jxx::lang::String>& value) : Super(), text_(value ? value : emptyText()) {}
    JTextComponent::~JTextComponent() = default;
    ::jxx::Ptr<::jxx::lang::String> JTextComponent::getText() const { return nativeComponent_ ? nativeComponent_->getText() : text_; }
    void JTextComponent::setText(const ::jxx::Ptr<::jxx::lang::String>& value) { text_ = value ? value : emptyText(); if(nativeComponent_) nativeComponent_->setText(text_); selectionStart_=selectionEnd_=caretPosition_=0; invalidate(); }
    ::jxx::lang::jbool JTextComponent::isEditable() const { return editable_; }
    void JTextComponent::setEditable(::jxx::lang::jbool value) { editable_=value; if(nativeComponent_) nativeComponent_->setEditable(value); }
    ::jxx::lang::jint JTextComponent::getCaretPosition() const { return caretPosition_; }
    void JTextComponent::setCaretPosition(::jxx::lang::jint value) { if(value<0) throw ::jxx::lang::IllegalArgumentException("position"); caretPosition_=selectionStart_=selectionEnd_=value; if(nativeComponent_) nativeComponent_->setCaretPosition(value); }
    void JTextComponent::select(::jxx::lang::jint start, ::jxx::lang::jint end) { if(start<0||end<0) throw ::jxx::lang::IllegalArgumentException("selection"); selectionStart_=std::min(start,end); selectionEnd_=std::max(start,end); caretPosition_=selectionEnd_; if(nativeComponent_) nativeComponent_->setSelection(selectionStart_,selectionEnd_); }
    void JTextComponent::selectAll() { selectionStart_=0; selectionEnd_=static_cast<::jxx::lang::jint>(getText()->utf8().size()); caretPosition_=selectionEnd_; if(nativeComponent_) nativeComponent_->setSelection(selectionStart_,selectionEnd_); }
    ::jxx::Ptr<::jxx::lang::String> JTextComponent::getSelectedText() const { const auto value=getText()->utf8(); const auto start=std::min<std::size_t>(selectionStart_,value.size()); const auto end=std::min<std::size_t>(selectionEnd_,value.size()); return ::jxx::NEW<::jxx::lang::String>(value.substr(start,end-start)); }
}
