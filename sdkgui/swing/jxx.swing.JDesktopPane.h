#pragma once
#include <vector>
#include "swing/jxx.swing.JLayeredPane.h"
namespace jxx::swing
{
    class JInternalFrame;
    class DesktopManager;
    class JDesktopPane : public ::jxx::lang::ClassBase<JDesktopPane, JLayeredPane>
    {
    public:
        using JxxSuper=JLayeredPane; using Super=::jxx::lang::ClassBase<JDesktopPane,JxxSuper>;
        static constexpr ::jxx::lang::jint LIVE_DRAG_MODE=0,OUTLINE_DRAG_MODE=1;
        JDesktopPane();
        ::jxx::Ptr<::jxx::awt::Component> add(const ::jxx::Ptr<::jxx::awt::Component>& component) override;
        void remove(const ::jxx::Ptr<::jxx::awt::Component>& component) override;
        std::vector<::jxx::Ptr<JInternalFrame>> getAllFrames() const;
        ::jxx::Ptr<JInternalFrame> getSelectedFrame() const; void setSelectedFrame(const ::jxx::Ptr<JInternalFrame>& frame);
        void setDesktopManager(const ::jxx::Ptr<DesktopManager>& manager); ::jxx::Ptr<DesktopManager> getDesktopManager() const;
        void setDragMode(::jxx::lang::jint mode); ::jxx::lang::jint getDragMode() const;
    private: ::jxx::Ptr<DesktopManager> desktopManager_;std::vector<::jxx::Ptr<JInternalFrame>> frames_;::jxx::Ptr<JInternalFrame> selected_;::jxx::lang::jint dragMode_=LIVE_DRAG_MODE;
    };
}
