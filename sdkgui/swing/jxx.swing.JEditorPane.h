#pragma once
#include "swing/text/jxx.swing.text.JTextComponent.h"
namespace jxx::swing
{
    class JEditorPane : public ::jxx::lang::ClassBase<JEditorPane, ::jxx::swing::text::JTextComponent>
    {
    public:
        using JxxSuper = ::jxx::swing::text::JTextComponent;
        using Super = ::jxx::lang::ClassBase<JEditorPane, JxxSuper>;
        JEditorPane();
        JEditorPane(const ::jxx::Ptr<::jxx::lang::String>& type,
            const ::jxx::Ptr<::jxx::lang::String>& text);
        explicit JEditorPane(const ::jxx::Ptr<::jxx::lang::String>& page);
        void setContentType(const ::jxx::Ptr<::jxx::lang::String>& type);
        ::jxx::Ptr<::jxx::lang::String> getContentType() const;
        void setPage(const ::jxx::Ptr<::jxx::lang::String>& page);
        ::jxx::Ptr<::jxx::lang::String> getPage() const;
    private:
        ::jxx::Ptr<::jxx::lang::String> contentType_;
        ::jxx::Ptr<::jxx::lang::String> page_;
    };
}
