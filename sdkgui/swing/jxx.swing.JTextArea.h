#pragma once
#include "awt/jxx.awt.Dimension.h"
#include "swing/jxx.swing.JTextComponent.h"
namespace jxx::swing
{
    class JTextArea : public ::jxx::lang::ClassBase<JTextArea,JTextComponent>
    {
    public:
        using JxxSuper=JTextComponent; using Super=::jxx::lang::ClassBase<JTextArea,JxxSuper>;
        JTextArea(); explicit JTextArea(const ::jxx::Ptr<::jxx::lang::String>& text); JTextArea(::jxx::lang::jint rows,::jxx::lang::jint columns); JTextArea(const ::jxx::Ptr<::jxx::lang::String>& text,::jxx::lang::jint rows,::jxx::lang::jint columns);
        void append(const ::jxx::Ptr<::jxx::lang::String>& text); void insert(const ::jxx::Ptr<::jxx::lang::String>& text,::jxx::lang::jint position); void replaceRange(const ::jxx::Ptr<::jxx::lang::String>& text,::jxx::lang::jint start,::jxx::lang::jint end);
        ::jxx::lang::jint getRows() const; void setRows(::jxx::lang::jint rows); ::jxx::lang::jint getColumns() const; void setColumns(::jxx::lang::jint columns);
        ::jxx::lang::jbool getLineWrap() const; void setLineWrap(::jxx::lang::jbool wrap); ::jxx::lang::jbool getWrapStyleWord() const; void setWrapStyleWord(::jxx::lang::jbool word);
        ::jxx::Ptr<::jxx::awt::Dimension> getPreferredScrollableViewportSize() const;
    private: ::jxx::lang::jint rows_=0,columns_=0; ::jxx::lang::jbool lineWrap_=false,wrapStyleWord_=false;
    };
}
