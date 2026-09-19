#include "swing/jxx.swing.JEditorPane.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::swing
{
    namespace
    {
        ::jxx::Ptr<::jxx::lang::String> plainText()
        { return ::jxx::NEW<::jxx::lang::String>("text/plain"); }
    }
    JEditorPane::JEditorPane() : Super(), contentType_(plainText()) {}
    JEditorPane::JEditorPane(const ::jxx::Ptr<::jxx::lang::String>& type,
        const ::jxx::Ptr<::jxx::lang::String>& text)
        : Super(text), contentType_(plainText()) { setContentType(type); }
    JEditorPane::JEditorPane(const ::jxx::Ptr<::jxx::lang::String>& page)
        : Super(), contentType_(plainText()) { setPage(page); }
    void JEditorPane::setContentType(const ::jxx::Ptr<::jxx::lang::String>& type)
    {
        if (!type) throw ::jxx::lang::NullPointerException("type");
        contentType_ = type;
    }
    ::jxx::Ptr<::jxx::lang::String> JEditorPane::getContentType() const
    { return contentType_; }
    void JEditorPane::setPage(const ::jxx::Ptr<::jxx::lang::String>& page)
    {
        if (!page) throw ::jxx::lang::NullPointerException("page");
        page_ = page;
    }
    ::jxx::Ptr<::jxx::lang::String> JEditorPane::getPage() const
    { return page_; }
}
