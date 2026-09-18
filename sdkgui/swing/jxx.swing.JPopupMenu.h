#pragma once
#include <vector>
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing
{
    class JMenuItem;
    class JPopupMenu : public ::jxx::lang::ClassBase<JPopupMenu, JComponent>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JPopupMenu, JxxSuper>;
        JPopupMenu();
        explicit JPopupMenu(const ::jxx::Ptr<::jxx::lang::String>& label);
        ::jxx::Ptr<JMenuItem> add(const ::jxx::Ptr<JMenuItem>& item);
        ::jxx::Ptr<JMenuItem> add(const ::jxx::Ptr<::jxx::lang::String>& text);
        void addSeparator();
        void insert(const ::jxx::Ptr<JMenuItem>& item, ::jxx::lang::jint index);
        void remove(::jxx::lang::jint index);
        void removeAll();
        ::jxx::lang::jint getComponentCount() const override;
        ::jxx::Ptr<::jxx::awt::Component> getComponent(::jxx::lang::jint index) const override;
        void setLabel(const ::jxx::Ptr<::jxx::lang::String>& label);
        ::jxx::Ptr<::jxx::lang::String> getLabel() const;
        void setInvoker(const ::jxx::Ptr<::jxx::awt::Component>& invoker);
        ::jxx::Ptr<::jxx::awt::Component> getInvoker() const;
        void show(const ::jxx::Ptr<::jxx::awt::Component>& invoker, ::jxx::lang::jint x, ::jxx::lang::jint y);
        void setVisible(::jxx::lang::jbool visible) override;
        ::jxx::lang::jbool isVisible() const;
    private:
        ::jxx::Ptr<::jxx::lang::String> label_;
        ::jxx::Ptr<::jxx::awt::Component> invoker_;
        std::vector<::jxx::Ptr<JMenuItem>> items_;
        ::jxx::lang::jint popupX_ = 0;
        ::jxx::lang::jint popupY_ = 0;
    };
}
