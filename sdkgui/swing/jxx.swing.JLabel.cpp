#include "swing/jxx.swing.JLabel.h"
#include "gui/internal/jxx.gui.internal.NativeComponent.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::swing
{
    namespace { ::jxx::Ptr<::jxx::lang::String> emptyText(){ return ::jxx::NEW<::jxx::lang::String>(""); } }
    JLabel::JLabel() : JLabel(emptyText(), LEADING) {}
    JLabel::JLabel(const ::jxx::Ptr<::jxx::lang::String>& value) : JLabel(value, LEADING) {}
    JLabel::JLabel(const ::jxx::Ptr<::jxx::lang::String>& value, ::jxx::lang::jint alignment)
        : text_(value ? value : emptyText()), alignment_(LEADING) { setHorizontalAlignment(alignment); }
    ::jxx::Ptr<::jxx::lang::String> JLabel::getText() const { return text_; }
    void JLabel::setText(const ::jxx::Ptr<::jxx::lang::String>& value)
    { text_ = value ? value : emptyText(); if (nativeComponent_) nativeComponent_->setText(text_); invalidate(); }
    ::jxx::lang::jint JLabel::getHorizontalAlignment() const { return alignment_; }
    void JLabel::setHorizontalAlignment(::jxx::lang::jint value)
    { if (value != LEFT && value != CENTER && value != RIGHT && value != LEADING && value != TRAILING) throw ::jxx::lang::IllegalArgumentException("alignment"); alignment_ = value; invalidate(); }
}
