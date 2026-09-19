#pragma once
#include "swing/jxx.swing.JEditorPane.h"
namespace jxx::swing
{
    class JTextPane : public ::jxx::lang::ClassBase<JTextPane, JEditorPane>
    {
    public:
        using JxxSuper = JEditorPane;
        using Super = ::jxx::lang::ClassBase<JTextPane, JxxSuper>;
        JTextPane();
        void replaceSelection(const ::jxx::Ptr<::jxx::lang::String>& content);
        void insertComponent(const ::jxx::Ptr<::jxx::awt::Component>& component);
        ::jxx::Ptr<::jxx::awt::Component> getLastInsertedComponent() const;
    private:
        ::jxx::Ptr<::jxx::awt::Component> lastInsertedComponent_;
    };
}
