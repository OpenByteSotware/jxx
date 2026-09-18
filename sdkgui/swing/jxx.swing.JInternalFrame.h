#pragma once
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.RootPaneContainer.h"
namespace jxx::swing
{
    class JRootPane;
    class JInternalFrame : public ::jxx::lang::ClassBase<JInternalFrame, JComponent, RootPaneContainer>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JInternalFrame, JxxSuper, RootPaneContainer>;
        JInternalFrame();
        explicit JInternalFrame(const ::jxx::Ptr<::jxx::lang::String>& title);
        JInternalFrame(const ::jxx::Ptr<::jxx::lang::String>& title, ::jxx::lang::jbool resizable, ::jxx::lang::jbool closable, ::jxx::lang::jbool maximizable, ::jxx::lang::jbool iconifiable);
        ::jxx::Ptr<JRootPane> getRootPane() const override;
        void setContentPane(const ::jxx::Ptr<::jxx::awt::Container>& pane) override;
        ::jxx::Ptr<::jxx::awt::Container> getContentPane() const override;
        void setLayeredPane(const ::jxx::Ptr<JLayeredPane>& pane) override;
        ::jxx::Ptr<JLayeredPane> getLayeredPane() const override;
        void setGlassPane(const ::jxx::Ptr<::jxx::awt::Component>& pane) override;
        ::jxx::Ptr<::jxx::awt::Component> getGlassPane() const override;
        void setTitle(const ::jxx::Ptr<::jxx::lang::String>& title); ::jxx::Ptr<::jxx::lang::String> getTitle() const;
        void setClosed(::jxx::lang::jbool value); ::jxx::lang::jbool isClosed() const;
        void setIcon(::jxx::lang::jbool value); ::jxx::lang::jbool isIcon() const;
        void setMaximum(::jxx::lang::jbool value); ::jxx::lang::jbool isMaximum() const;
        void setSelected(::jxx::lang::jbool value); ::jxx::lang::jbool isSelected() const;
        ::jxx::lang::jbool isResizable() const; ::jxx::lang::jbool isClosable() const; ::jxx::lang::jbool isMaximizable() const; ::jxx::lang::jbool isIconifiable() const;
        void dispose(); void moveToFront(); void moveToBack();
    private:
        ::jxx::Ptr<JRootPane> rootPane_; ::jxx::Ptr<::jxx::lang::String> title_;
        ::jxx::lang::jbool resizable_=false,closable_=false,maximizable_=false,iconifiable_=false,closed_=false,icon_=false,maximum_=false,selected_=false;
    };
}
