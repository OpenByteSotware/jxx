#pragma once
#include <vector>
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.SwingConstants.h"
namespace jxx::swing
{
    class JTabbedPane : public ::jxx::lang::ClassBase<JTabbedPane,JComponent,SwingConstants>
    {
    public:
        using JxxSuper=JComponent; using Super=::jxx::lang::ClassBase<JTabbedPane,JxxSuper,SwingConstants>;
        JTabbedPane(); explicit JTabbedPane(::jxx::lang::jint placement);
        void addTab(const ::jxx::Ptr<::jxx::lang::String>& title,const ::jxx::Ptr<::jxx::awt::Component>& component);
        void insertTab(const ::jxx::Ptr<::jxx::lang::String>& title,const ::jxx::Ptr<::jxx::awt::Component>& component,::jxx::lang::jint index);
        void removeTabAt(::jxx::lang::jint index); ::jxx::lang::jint getTabCount() const;
        ::jxx::Ptr<::jxx::lang::String> getTitleAt(::jxx::lang::jint index) const; void setTitleAt(::jxx::lang::jint index,const ::jxx::Ptr<::jxx::lang::String>& title);
        ::jxx::Ptr<::jxx::awt::Component> getComponentAt(::jxx::lang::jint index) const;
        void setSelectedIndex(::jxx::lang::jint index); ::jxx::lang::jint getSelectedIndex() const;
        ::jxx::Ptr<::jxx::awt::Component> getSelectedComponent() const;
        void setTabPlacement(::jxx::lang::jint placement); ::jxx::lang::jint getTabPlacement() const;
    private:
        struct Tab{::jxx::Ptr<::jxx::lang::String> title;::jxx::Ptr<::jxx::awt::Component> component;};
        std::vector<Tab> tabs_; ::jxx::lang::jint selectedIndex_=-1,placement_=TOP;
    };
}
