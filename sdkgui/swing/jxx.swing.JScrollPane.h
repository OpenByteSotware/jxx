#pragma once
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.ScrollPaneConstants.h"
namespace jxx::swing
{
    class JScrollBar; class JViewport;
    class JScrollPane : public ::jxx::lang::ClassBase<JScrollPane,JComponent,ScrollPaneConstants>
    {
    public:
        using JxxSuper=JComponent; using Super=::jxx::lang::ClassBase<JScrollPane,JxxSuper,ScrollPaneConstants>;
        JScrollPane(); explicit JScrollPane(const ::jxx::Ptr<::jxx::awt::Component>& view);
        JScrollPane(const ::jxx::Ptr<::jxx::awt::Component>& view,::jxx::lang::jint verticalPolicy,::jxx::lang::jint horizontalPolicy);
        void setViewport(const ::jxx::Ptr<JViewport>& viewport); ::jxx::Ptr<JViewport> getViewport() const;
        void setViewportView(const ::jxx::Ptr<::jxx::awt::Component>& view);
        void setVerticalScrollBarPolicy(::jxx::lang::jint policy); ::jxx::lang::jint getVerticalScrollBarPolicy() const;
        void setHorizontalScrollBarPolicy(::jxx::lang::jint policy); ::jxx::lang::jint getHorizontalScrollBarPolicy() const;
        ::jxx::Ptr<JScrollBar> getVerticalScrollBar() const; ::jxx::Ptr<JScrollBar> getHorizontalScrollBar() const;
    private:
        ::jxx::Ptr<JViewport> viewport_; ::jxx::Ptr<JScrollBar> vertical_; ::jxx::Ptr<JScrollBar> horizontal_;
        ::jxx::lang::jint verticalPolicy_=VERTICAL_SCROLLBAR_AS_NEEDED,horizontalPolicy_=HORIZONTAL_SCROLLBAR_AS_NEEDED;
    };
}
