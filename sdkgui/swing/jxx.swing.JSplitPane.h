#pragma once
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.SwingConstants.h"
namespace jxx::swing
{
    class JSplitPane : public ::jxx::lang::ClassBase<JSplitPane,JComponent,SwingConstants>
    {
    public:
        using JxxSuper=JComponent; using Super=::jxx::lang::ClassBase<JSplitPane,JxxSuper,SwingConstants>;
        JSplitPane(); explicit JSplitPane(::jxx::lang::jint orientation);
        JSplitPane(::jxx::lang::jint orientation,const ::jxx::Ptr<::jxx::awt::Component>& first,const ::jxx::Ptr<::jxx::awt::Component>& second);
        void setOrientation(::jxx::lang::jint orientation); ::jxx::lang::jint getOrientation() const;
        void setLeftComponent(const ::jxx::Ptr<::jxx::awt::Component>& component); ::jxx::Ptr<::jxx::awt::Component> getLeftComponent() const;
        void setRightComponent(const ::jxx::Ptr<::jxx::awt::Component>& component); ::jxx::Ptr<::jxx::awt::Component> getRightComponent() const;
        void setTopComponent(const ::jxx::Ptr<::jxx::awt::Component>& component); ::jxx::Ptr<::jxx::awt::Component> getTopComponent() const;
        void setBottomComponent(const ::jxx::Ptr<::jxx::awt::Component>& component); ::jxx::Ptr<::jxx::awt::Component> getBottomComponent() const;
        void setDividerLocation(::jxx::lang::jint location); ::jxx::lang::jint getDividerLocation() const;
        void setResizeWeight(::jxx::lang::jdouble weight); ::jxx::lang::jdouble getResizeWeight() const;
        void doLayout() override;
    private:
        void setFirst(const ::jxx::Ptr<::jxx::awt::Component>& component); void setSecond(const ::jxx::Ptr<::jxx::awt::Component>& component);
        ::jxx::lang::jint orientation_=HORIZONTAL,dividerLocation_=-1; ::jxx::lang::jdouble resizeWeight_=0.0;
        ::jxx::Ptr<::jxx::awt::Component> first_,second_;
    };
}
